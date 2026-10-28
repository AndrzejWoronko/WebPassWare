#ifndef CORMREPOSITORY_H
#define CORMREPOSITORY_H

#include "Global.h"
#include "Database.h"
#include "OrmObject.h"
#include "ExceptionSql.h"

#include <memory>

namespace wpw {

// What the services need from a table of ModelName.
template<class ModelName>
class IRepository
{
public:
    virtual ~IRepository() = default;

    virtual ModelName* prototype() = 0;
    virtual QList<ModelName*> findAll(const QString &orderby = QString()) = 0;
    virtual QList<ModelName*> findBy(const QString &fieldName, const QVariant value) = 0;
    virtual QList<ModelName*> findByAnd(const QHash<QString, QVariant> &params) = 0;
    virtual ModelName* first() = 0;
    virtual ModelName* find(qint64 id) = 0;
    virtual qint64 add(ModelName *object, qint64 newId = 0) = 0;
    virtual bool update(ModelName *object) = 0;
    virtual bool remove(qint64 id) = 0;
    virtual bool markDeleted(qint64 id) = 0;
};

// The table of ModelName in a database: schema and records. The models
// only describe their columns (COrmObject); every SQL statement goes
// through the CDatabase given here. Finders return new objects the caller
// owns. Database errors are thrown as CExceptionSql.
template<class ModelName>
class COrmRepository : public IRepository<ModelName>
{
    CDatabase &m_db;
    std::unique_ptr<ModelName> m_object;

    QString tableName() const
    {
        return m_object->getTableName();
    }

    // Conditions carry placeholders only; the values go in bindings.
    QString idCondition() const
    {
        return QString(" WHERE id_%1 = :wid").arg(tableName());
    }

    static QVariantHash idBinding(qint64 id)
    {
        QVariantHash bindings;
        bindings.insert(":wid", id);
        return bindings;
    }

    // Column names come from callers (finders, sort order). Only the model's
    // own columns may reach the SQL text; like SQLite, case is ignored.
    QString checkedColumn(const QString &name) const
    {
        const QString column = name.trimmed();
        const QString idColumn = QString("id_%1").arg(tableName());
        if (column.compare(idColumn, Qt::CaseInsensitive) == 0)
        {
            return idColumn;
        }
        const QMetaObject *meta = m_object->metaObject();
        // Property 0 is QObject::objectName, not a column.
        for (int i = 1; i < meta->propertyCount(); i++)
        {
            const QString property = QString::fromLatin1(meta->property(i).name());
            if (column.compare(property, Qt::CaseInsensitive) == 0)
            {
                return property;
            }
        }
        throw CExceptionSql(Q_FUNC_INFO, name, QString("Unknown column of %1: %2").arg(tableName(), name));
    }

    // "col[ ASC|DESC][, ...]" with checked columns, as an ORDER BY clause.
    QString orderByClause(const QString &orderby) const
    {
        if (orderby.trimmed().isEmpty())
        {
            return QString();
        }
        QStringList terms;
        const QStringList requested = orderby.split(',');
        for (const QString &term : requested)
        {
            const QStringList parts = term.simplified().split(' ');
            QString checked = checkedColumn(parts.value(0));
            if (parts.size() == 2 && (parts.at(1).compare("ASC", Qt::CaseInsensitive) == 0
                                      || parts.at(1).compare("DESC", Qt::CaseInsensitive) == 0))
            {
                checked += " " + parts.at(1).toUpper();
            }
            else if (parts.size() != 1)
            {
                throw CExceptionSql(Q_FUNC_INFO, orderby, QString("Invalid sort order: %1").arg(orderby));
            }
            terms.append(checked);
        }
        return " ORDER BY " + terms.join(", ");
    }

    // "WHERE a = :w0 <joiner> b = :w1 ..." for the given column/value pairs.
    QString whereClause(const QList<QPair<QString, QVariant>> &conditions, const QString &joiner, QVariantHash &bindings) const
    {
        QStringList parts;
        for (int i = 0; i < conditions.size(); i++)
        {
            const QString placeholder = QString(":w%1").arg(i);
            parts.append(QString("%1 = %2").arg(checkedColumn(conditions.at(i).first), placeholder));
            bindings.insert(placeholder, conditions.at(i).second);
        }
        return " WHERE " + parts.join(" " + joiner + " ");
    }

    static QList<QPair<QString, QVariant>> pairs(const QHash<QString, QVariant> &params)
    {
        QList<QPair<QString, QVariant>> result;
        for (auto it = params.constBegin(); it != params.constEnd(); ++it)
        {
            result.append(qMakePair(it.key(), it.value()));
        }
        return result;
    }

    QList<ModelName*> toObjects(const QList<QSqlRecord> &records) const
    {
        QList<ModelName*> objects;
        for (const QSqlRecord &record : records)
        {
            objects.append(COrmObject<ModelName>::template translateRecToObj<ModelName>(record));
        }
        return objects;
    }

public:
    explicit COrmRepository(CDatabase &db) : m_db(db), m_object(std::make_unique<ModelName>())
    {
    }

    CDatabase &database()
    {
        return m_db;
    }

    // An empty object that describes the table (name, columns, indexes).
    ModelName* prototype() override final
    {
        return m_object.get();
    }

    // Schema

    bool createTable()
    {
        return m_db.createTable(tableName(), m_object->getSqlDescriptions());
    }

    bool alterTableAddColumn(const QString &name)
    {
        return m_db.alterTableAddColumn(tableName(), m_object->getColumnFromModel(name));
    }

    bool createIndexes()
    {
        return m_db.createTableIndexes(tableName(), m_object->getIndexFields(), false);
    }

    bool createUniqueIndexes()
    {
        return m_db.createTableIndexes(tableName(), m_object->getUniqueIndexFields(), true);
    }

    bool dropTable()
    {
        return m_db.dropTable(tableName());
    }

    bool dropIndexes()
    {
        m_db.dropTableIndexes(tableName(), m_object->getIndexFields());
        return m_db.dropTableIndexes(tableName(), m_object->getUniqueIndexFields());
    }

    QList<QSqlIndex> getTableIndexes()
    {
        return m_db.getTableIndexes(tableName());
    }

    QHash<QString, QString> getTableIndexesNames()
    {
        QHash<QString, QString> result;
        const QList<QSqlIndex> indexes = m_db.getTableIndexes(tableName());
        for (const QSqlIndex &index : indexes)
        {
            result.insert(index.cursorName(), index.name());
        }
        return result;
    }

    // Records

    /*!
       Inserts \a object with all its properties (deleted = false, timestp =
       now). With \a force_id the record gets \a newId. Returns true and sets
       the object's id on success.
    */
    bool save(ModelName *object, qint64 newId = 0, bool force_id = false)
    {
        object->setObjectValues();
        object->updateUnsavedAllFields();
        object->markSaved(m_db.addRecord(tableName(), object->changedValues(), newId, force_id));
        return object->getId() >= 0;
    }

    /*!
       Writes the properties of \a object changed since it was saved or
       loaded. Returns false for an object without id.
    */
    bool update(ModelName *object) override final
    {
        if (object->getId() < 0)
        {
            return false;
        }
        if (object->getPropertiesForUpdate().isEmpty())
        {
            return true;
        }

        const bool ret = m_db.updateRecord(tableName(), object->changedValues(), idCondition(), idBinding(object->getId()));
        if (ret)
        {
            object->clearUpdateList();
        }
        return ret;
    }

    // Writes one column of \a object at once.
    bool updateProperty(ModelName *object, const QString &fieldName, const QVariant &value)
    {
        if (object->getId() < 0)
        {
            return false;
        }
        QHash<QString, QVariant> info;
        info.insert(fieldName, value);
        if (!m_db.updateRecord(tableName(), info, idCondition(), idBinding(object->getId())))
        {
            return false;
        }
        object->markPropertyStored(fieldName, value);
        return true;
    }

    // Deletes the record of \a object; its id becomes -1.
    bool remove(ModelName *object)
    {
        const bool ret = m_db.remove(tableName(), idCondition(), idBinding(object->getId()));
        if (ret)
        {
            object->setId(-1);
        }
        return ret;
    }

    // Marks the record of \a object deleted; its id becomes -1.
    bool setDeleted(ModelName *object)
    {
        const bool ret = m_db.setDeleted(tableName(), idCondition(), idBinding(object->getId()));
        if (ret)
        {
            object->setId(-1);
        }
        return ret;
    }

    bool removeAll()
    {
        return m_db.remove(tableName(), "");
    }

    bool deleteAll()
    {
        return m_db.setDeleted(tableName(), "");
    }

    // Finders

    ModelName* find(qint64 id) override final
    {
        const QList<QSqlRecord> list = m_db.find(tableName(), "*", idCondition(), idBinding(id));
        if (list.isEmpty())
        {
            return nullptr;
        }
        return COrmObject<ModelName>::template translateRecToObj<ModelName>(list.first());
    }

    QList<ModelName*> findAll(const QString &orderby = QString()) override final
    {
        return toObjects(m_db.find(tableName(), "*", orderByClause(orderby)));
    }

    ModelName* first() override final
    {
        const QString pkName = QString("id_%1").arg(tableName());
        const QSqlRecord record = m_db.first(tableName(), pkName);
        if (record.isNull(pkName))
        {
            return nullptr;
        }
        return COrmObject<ModelName>::template translateRecToObj<ModelName>(record);
    }

    ModelName* last()
    {
        const QString pkName = QString("id_%1").arg(tableName());
        const QSqlRecord record = m_db.last(tableName(), pkName);
        if (record.isNull(pkName))
        {
            return nullptr;
        }
        return COrmObject<ModelName>::template translateRecToObj<ModelName>(record);
    }

    QList<ModelName*> findBy(const QString &fieldName, const QVariant value) override final
    {
        return findBy(fieldName, value, QString());
    }

    QList<ModelName*> findBy(const QString &fieldName, const QVariant value, const QString &orderby)
    {
        QVariantHash bindings;
        const QString where = whereClause({qMakePair(fieldName, value)}, "AND", bindings);
        return toObjects(m_db.find(tableName(), "*", where + orderByClause(orderby), bindings));
    }

    QList<ModelName*> findBy(const QString &fieldName, const QVector<QVariant> &values, const QString &orderby = QString())
    {
        if (values.isEmpty())
        {
            return QList<ModelName*>();
        }
        QList<QPair<QString, QVariant>> conditions;
        for (const QVariant &value : values)
        {
            conditions.append(qMakePair(fieldName, value));
        }
        QVariantHash bindings;
        const QString where = whereClause(conditions, "OR", bindings);
        return toObjects(m_db.find(tableName(), "*", where + orderByClause(orderby), bindings));
    }

    QList<ModelName*> findByOr(const QHash<QString, QVariant> &params)
    {
        if (params.isEmpty())
        {
            return QList<ModelName*>();
        }
        QVariantHash bindings;
        const QString where = whereClause(pairs(params), "OR", bindings);
        return toObjects(m_db.find(tableName(), "*", where, bindings));
    }

    QList<ModelName*> findByAnd(const QHash<QString, QVariant> &params) override final
    {
        if (params.isEmpty())
        {
            return QList<ModelName*>();
        }
        QVariantHash bindings;
        const QString where = whereClause(pairs(params), "AND", bindings);
        return toObjects(m_db.find(tableName(), "*", where, bindings));
    }

    // True if the table has any record.
    bool existsTable()
    {
        return !m_db.find(tableName(), "*", "").isEmpty();
    }

    bool exists(qint64 id)
    {
        return !m_db.find(tableName(), "*", idCondition(), idBinding(id)).isEmpty();
    }

    qint64 count()
    {
        return m_db.count(tableName(), "*");
    }

    // Number of records with a non-null \a fieldName.
    qint64 count(const QString &fieldName)
    {
        return m_db.count(tableName(), fieldName);
    }

    // IRepository

    qint64 add(ModelName *object, qint64 newId = 0) override final
    {
        save(object, newId);
        return object->getId();
    }

    bool remove(qint64 id) override final
    {
        std::unique_ptr<ModelName> object(find(id));
        return object && remove(object.get());
    }

    bool markDeleted(qint64 id) override final
    {
        std::unique_ptr<ModelName> object(find(id));
        return object && setDeleted(object.get());
    }
};

} // namespace wpw

#endif // CORMREPOSITORY_H
