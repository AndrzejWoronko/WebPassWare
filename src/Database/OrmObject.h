#ifndef CORMOBJECT_H
#define CORMOBJECT_H

#include <QDateTime>
#include <QMetaProperty>
#include <QMetaType>
#include <QSqlRecord>
#include "Global.h"
#include "Tools.h"
#include "Database.h"
#include "ExceptionSql.h"

#define ORM_PROPERTY(type, name) \
Q_PROPERTY(type name READ get##name WRITE set##name) \
    private: \
    type name; \
    const bool m_declaredType_##name = registerDeclaredTypeName(#name, #type); \
    public: \
    type get##name() const { return name;} \
    void set##name(const type & input_##name)  \
{ \
        name = input_##name; \
        m_hasUnsavedChanges = true; \
        if(!m_propertiesForUpdate.contains(#name)) \
        m_propertiesForUpdate.append(#name); \
}

#define ADD_SQL_DESC(name, desc) setSqlDescription( QString(name), QString(desc));

#define ADD_VARCHAR(name, value) setVarCharDescription( QString(name), value);

#define ADD_SQL_INDEX(name, desc) addIndex( QString(name), QString(desc));

#define ADD_SQL_UNIQ_INDEX(name, desc) addUniqueIndex( QString(name), QString(desc));



namespace wpw {

template<class ModelName>
class COrmObject : public QObject
{
    ORM_PROPERTY(bool, deleted) //Znacznik usunięcia
    ORM_PROPERTY(Timestamp, timestp) //Czas ostatniej modyfikacji timestamp

public:
    qint64 m_id;  // Object id

    explicit COrmObject(QObject *parent = nullptr) : QObject(parent)
    {
        m_id = -1;
        clearUpdateList();
        m_sqlDescriptions.clear();
        m_tableTypes.clear();
        m_indexFields.clear();
        m_uniqueIndexFields.clear();

        ADD_SQL_DESC("deleted", "BOOL NOT NULL DEFAULT FALSE")
        ADD_SQL_DESC("timestp", "TIMESTAMP NOT NULL")

        m_hasUnsavedChanges = false;
        //Type register
        qRegisterMetaType<Varchar255>("Varchar255"); //Varchar
        qRegisterMetaType<Timestamp>("Timestamp");   //Date and time
        qRegisterMetaType<Color>("Color");           //Color
    }

    void initORMObject()
    {
        setTableName();
        initSqlDescription();
        bzero(); //Init default values
        clearUpdateList();
    }

    // Table of a model class: "wpw::PassEntry" -> "pass_entry" (the namespace
    // is not part of the table name, which must stay as in existing files).
    static QString tableNameOf(const QMetaObject *meta)
    {
        QString className = QString::fromLatin1(meta->className());
        className = className.mid(className.lastIndexOf(QLatin1String("::")) + 1);
        if (className.startsWith(QLatin1Char(':')))
        {
            className.remove(0, 1);
        }
        return Tools::tableize(className);
    }

    void setTableName()
    {
        m_tableName = tableNameOf(metaObject());
    }

    void setTableName(const QString & tableName)
    {
        m_tableName = tableName;
        m_tableName = Tools::tableize(m_tableName);
    }

    QString getTableName() const
    {
        return m_tableName;
    }

    void addIndex(QString const &key, QString const &value)
    {
        m_indexFields.insert(key, value);
    }

    void addUniqueIndex(QString const &key, QString const &value)
    {
        m_uniqueIndexFields.insert(key, value);
    }

    void clearUpdateList()
    {
        m_propertiesForUpdate.clear();
        m_hasUnsavedChanges = false;
    }

    // Property type names as written in ORM_PROPERTY. The column type depends
    // on the typedef (Varchar255, Timestamp, Price), and QMetaProperty::typeName()
    // reports the underlying type instead in Qt 6 (QString, QDateTime, double).
    static QHash<QString, QString> &declaredTypeNames()
    {
        static QHash<QString, QString> names;
        return names;
    }

    static bool registerDeclaredTypeName(const char *name, const char *type)
    {
        declaredTypeNames().insert(QString::fromLatin1(name), QString::fromLatin1(type));
        return true;
    }

    void initSqlDescription()
    {
        QString propertyName;
        QString typeName;
        QString propertyValue;

        CDatabase::fillTableTypes(m_tableTypes);

        //Loop from 1 to skip objectName
        for(int i = 1; i < metaObject()->propertyCount(); i++)
        {
            propertyName = QString::fromLatin1(metaObject()->property(i).name());
            typeName = declaredTypeNames().value(propertyName, QString::fromLatin1(metaObject()->property(i).typeName()));
            propertyValue = m_tableTypes.value(typeName);
            if (propertyValue.isNull())
                DEBUG_WITH_LINE << typeName;
            setSqlDescription(propertyName, propertyValue);
        }
    }

    void bzero(void)
    {
        for(int i = 1; i < metaObject()->propertyCount(); i++)
        {
            switch (metaObject()->property(i).userType())
            {
            case QMetaType::QTime:
                metaObject()->property(i).write(this, QVariant(QTime::currentTime()));
            break;
            case QMetaType::QDate:
            case QMetaType::QDateTime:
                metaObject()->property(i).write(this, QVariant(QDateTime::currentDateTime()));
                break;
            case QMetaType::Int:
            case QMetaType::UInt:
            case QMetaType::LongLong:
            case QMetaType::ULongLong:
                metaObject()->property(i).write(this, QVariant(0));
                break;
            case QMetaType::Double:
                metaObject()->property(i).write(this, QVariant(0.00));
                break;
            case QMetaType::QChar:
            case QMetaType::QString:
            case QMetaType::QByteArray:
                metaObject()->property(i).write(this, QVariant(""));
                break;
            case QMetaType::Bool:
                metaObject()->property(i).write(this, QVariant(false));
                break;
            default: // Default type int
                DEBUG_WITH_LINE << "Unknown property name:" << metaObject()->property(i).name() << " type name: " << metaObject()->property(i).typeName() << " variant type: " << metaObject()->property(i).userType();
                metaObject()->property(i).write(this, QVariant(0));
                break;
            }
        }
    }

    void setSqlDescription(QString const &key, QString const &value)
    {
        PkNames field;
        field.first = key;
        field.second = value;

        for(int i = 0; i < m_sqlDescriptions.size(); i++)
        {
            if (m_sqlDescriptions.at(i).first == field.first)
            {
                m_sqlDescriptions.replace(i, field);
                return;
            }
        }
        m_sqlDescriptions << field;
    }

    void setVarCharDescription(QString const &key, int value)
    {
        QString varCharDescription = QString("VARCHAR(%1) NOT NULL DEFAULT ''").arg(value);

        setSqlDescription(key, varCharDescription);
    }

    /*!
       Sets new id.
    */
    void setId(qint64 newId)
    {
        m_id = newId;
        m_hasUnsavedChanges = true;
    }
    /*!
       Returns object id.
     */
    qint64 getId() const
    {
        return m_id;
    }

    bool hasUnsavedChanges()
    {
        return m_hasUnsavedChanges;
    }

    void updateUnsavedAllFields()
    {
        QString propertyName;

        m_hasUnsavedChanges = true;
        for(int i = 1; i < metaObject()->propertyCount(); i++)
        {
            propertyName = QString::fromLatin1(metaObject()->property(i).name());
            if(!m_propertiesForUpdate.contains(propertyName))
                m_propertiesForUpdate.append(propertyName);
        }
    }

    void setObjectValues(bool newDeleted = false, QDateTime newTimeStp = QDateTime::currentDateTime())
    {
        this->setdeleted(newDeleted);
        this->settimestp(newTimeStp);
    }

    virtual QString toString()
    {       
        QString str;        
        str += m_tableName;
        str += '\n';
        str.append(QString("    id_%1 : ").arg(m_tableName) + QString::number(m_id) + '\n');
        for(int i = 1; i < metaObject()->propertyCount(); i++)
            str.append(QString("    ") + metaObject()->property(i).name() +
                       QString(" : ") + property(metaObject()->property(i).name()).toString() + '\n');
        return str;
    }

    PkNames getColumnFromModel(const QString &name)
    {
        PkNames field;
        field.first = QString("");
        field.second = QString("");
        for (auto f : std::as_const(m_sqlDescriptions))
        {
            if (f.first == name)
            {
                field = f;
                break;
            }
        }
        return field;
    }

    QHash <QString, QString> getIndexFields()
    {
        return m_indexFields;
    }

    QHash <QString, QString> getUniqueIndexFields()
    {
        return m_uniqueIndexFields;
    }

protected:
    bool m_hasUnsavedChanges;
    QStringList m_propertiesForUpdate;
    QString m_tableName;

    QList <PkNames> m_sqlDescriptions;
    QHash <QString, QString> m_tableTypes;
    QHash <QString, QString> m_indexFields;
    QHash <QString, QString> m_uniqueIndexFields;

public:
    QList<PkNames> getSqlDescriptions() const
    {
        return m_sqlDescriptions;
    }

    QStringList getPropertiesForUpdate() const
    {
        return m_propertiesForUpdate;
    }

    // Column values of the properties changed since the last save or load.
    QHash<QString, QVariant> changedValues()
    {
        QHash<QString, QVariant> info;
        for (const QString &propertyName : std::as_const(m_propertiesForUpdate))
        {
            QVariant propertyValue;
            if (propertyName == "timestp")
                propertyValue = this->gettimestp();
            else if (propertyName == "deleted")
                propertyValue = this->getdeleted();
            else
                propertyValue = property(qPrintable(propertyName));
            info.insert(propertyName, propertyValue);
        }
        return info;
    }

    // After an INSERT: the new id, nothing left to save.
    void markSaved(qint64 newId)
    {
        m_id = newId;
        if (m_id >= 0)
        {
            clearUpdateList();
        }
    }

    // After UPDATE of one column: the property holds the stored value.
    void markPropertyStored(const QString &fieldName, const QVariant &value)
    {
        setProperty(qPrintable(fieldName), value);
        m_propertiesForUpdate.removeOne(fieldName);
    }

    // Builds a new object from a table record; the caller owns it.
    template<class T>
    static T* translateRecToObj(const QSqlRecord &record)
    {
        T *result = new T;
        QString currentFieldName;        
        QString tableName = tableNameOf(result->metaObject());

        for(int i = 1; i < result->metaObject()->propertyCount(); i++)
        {
            currentFieldName = QString(result->metaObject()->property(i).name());
            if(record.contains(currentFieldName))
                result->metaObject()->property(i).write(result, record.value(currentFieldName));
            else if(record.contains(currentFieldName.toLower()))
                result->metaObject()->property(i).write(result, record.value(currentFieldName.toLower()));
        }
        result->setId(record.value(QString("id_%1").arg(tableName)).toLongLong());
        //BASIC REC INFO
        result->setdeleted(record.value("deleted").toBool());
        result->settimestp(record.value("timestp").toDateTime());
        result->clearUpdateList();
        return result;
    }
};

} // namespace wpw

#endif // CORMOBJECT_H
