#ifndef CORMOBJECT_H
#define CORMOBJECT_H

#include <QDateTime>
#include <QMetaProperty>
#include <QMetaType>
#include <QSqlRecord>
#include "Global.h"
#include "Tools.h"
#include "Database.h"

#define ORM_PROPERTY(type, name) \
Q_PROPERTY(type name READ get##name WRITE set##name) \
    private: \
    type name; \
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



template<class ModelName>
class COrmObject : public QObject
{
    ORM_PROPERTY(bool, deleted) //Znacznik usunięcia
    ORM_PROPERTY(Timestamp, timestp) //Czas ostatniej modyfikacji timestamp

public:
    qint64 m_id;  // Object id

    explicit COrmObject(QObject *parent = NULL) : QObject(parent)
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

    void setTableName()
    {
        m_tableName = metaObject()->className();
        m_tableName = Tools::tableize(m_tableName);
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

    void initSqlDescription()
    {
        QString propertyName;
        QString typeName;
        QString propertyValue;

        setTableName();
        DB.fillTableTypes(m_tableTypes);

        //Loop from 1 to skip objectName
        for(int i = 1; i < metaObject()->propertyCount(); i++)
        {
            propertyName = QString::fromLatin1(metaObject()->property(i).name());
            typeName = QString::fromLatin1(metaObject()->property(i).typeName());
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
            switch (metaObject()->property(i).type())
            {
            case QVariant::Time:
                metaObject()->property(i).write(this, QVariant(QTime::currentTime()));
            break;
            case QVariant::Date:
            case QVariant::DateTime:            
                metaObject()->property(i).write(this, QVariant(QDateTime::currentDateTime()));
                break;
            case QVariant::Int:
            case QVariant::UInt:
            case QVariant::LongLong:
            case QVariant::ULongLong:
                metaObject()->property(i).write(this, QVariant(0));
                break;
            case QVariant::Double:
                metaObject()->property(i).write(this, QVariant(0.00));
                break;
            case QVariant::Char:
            case QVariant::String:            
            case QVariant::ByteArray:
                metaObject()->property(i).write(this, QVariant(""));
                break;
            case QVariant::Bool:
                metaObject()->property(i).write(this, QVariant(false));
                break;
            default: // Default type int
                DEBUG_WITH_LINE << "Unknown property name:" << metaObject()->property(i).name() << " type name: " << metaObject()->property(i).typeName() << " variant type: " << metaObject()->property(i).type();
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
       Creates table associated with model.
      Returns true if table created, otherwise return false.
    */
    bool createTable()
    {
        bool ret = false;
        setTableName();
        try
        {
            ret = DB.createTable(m_tableName, m_sqlDescriptions);
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    bool alterTableAddColumn(const QString &name)
    {
        bool ret = false;
        setTableName();
        PkNames field = getColumnFromModel(name);
        try
        {
            ret =  DB.alterTableAddColumn(m_tableName, field);
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    /*!
       Creates indexes to table associated with model.

    Returns true if table created, otherwise return false.
                                       */
    bool createIndexes()
    {
        bool ret = false;
        setTableName();
        try
        {
           ret =  DB.createTableIndexes(m_tableName, m_indexFields, false);
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    bool createUniqueIndexes()
    {
        bool ret = false;
        setTableName();
        try
        {
            ret =  DB.createTableIndexes(m_tableName, m_uniqueIndexFields, true);
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    bool dropTable() const
    {
        //setTableName();
        //return CDatabase::adapter->dropTable(m_tableName);
        bool ret = false;
        try
        {
            ret =  DB.dropTable(m_tableName);
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    bool dropIndexes()
    {
        bool ret = false;
        setTableName();
        try
        {
            ret = DB.dropTableIndexes(m_tableName, m_indexFields);
            ret = DB.dropTableIndexes(m_tableName, m_uniqueIndexFields);
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    bool beginTransaction()
    {
        bool ret = false;
        try
        {
            ret =  DB.beginTransaction();
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    bool commitTransaction()
    {
        bool ret = false;
        try
        {
            ret =  DB.commitTransaction();
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    bool rollbackTransaction()
    {
        bool ret = false;
        try
        {
            ret = DB.rollbackTransaction();
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
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

    /*!
       Creates new record in table. Read all meta-property from model that have be changed and save their into table.
       Returns true if success, otherwise return false.
    */

    bool save(qint64 newId = 0, const QString &tableName = QString())
    {
        qint64 id = -1;
        QHash<QString, QVariant> info;
        QString propertyName;
        QVariant propertyValue;
        setObjectValues();
        updateUnsavedAllFields();
        for(int i = 0; i < m_propertiesForUpdate.size(); i++)
        {
            propertyName = m_propertiesForUpdate.value(i);

            if (propertyName == "timestp")
                propertyValue = this->gettimestp();
            else if (propertyName == "deleted")
                propertyValue = this->getdeleted();
            else
                propertyValue = property(qPrintable(propertyName));
            info.insert(propertyName, propertyValue);
        }

        if (tableName.isEmpty())
            setTableName();
        else
            setTableName(tableName);
        try
        {
            id = DB.addRecord(m_tableName, info, newId);
        }
        catch (CExceptionSql *e)
        {
        }
        m_id = id;
        if(m_id >= 0)
        {
            clearUpdateList();
        }
        return (m_id >= 0);
    }

    /*!
       Updates existing record in table with object's \a id.
       Returns true if success, otherwise return false.
   */
    bool update(const QString &tableName = QString())
    {
        QHash<QString, QVariant> info;
        QString propertyName;
        QVariant propertyValue;

        if(m_id < 0)
            return false;
        if(m_propertiesForUpdate.isEmpty())
            return true;

        for(int i = 0; i < m_propertiesForUpdate.size(); i++)
        {
            propertyName = m_propertiesForUpdate.value(i);

            if (propertyName == "timestp")
                propertyValue = this->gettimestp();
            else if (propertyName == "deleted")
                propertyValue = this->getdeleted();
            else
                propertyValue = property(qPrintable(propertyName));
            info.insert(propertyName, propertyValue);
        }
        if (tableName.isEmpty())
            setTableName();
        else
            setTableName(tableName);

        bool ret = false;
        try
        {
            ret = DB.updateRecord(m_tableName, info, getIdCondition(m_id));
        }
        catch (CExceptionSql *e)
        {
        }
        if (ret)
            clearUpdateList();
        return ret;
    }

    QString getIdCondition(qint64 id)
    {
        QString where = QString(" WHERE id_%1 = %2").arg(m_tableName).arg(id);
        return where;
    }

    virtual QString toString()
    {
        QString str;
        setTableName();
        str += m_tableName;
        str += '\n';
        str.append(QString("    id_%1 : ").arg(m_tableName) + QString::number(m_id) + '\n');
        for(int i = 1; i < metaObject()->propertyCount(); i++)
            str.append(QString("    ") + metaObject()->property(i).name() +
                       QString(" : ") + property(metaObject()->property(i).name()).toString() + '\n');
        return str;
    }

    /*!
       Finds object by \a id.
       Returns pointer to found model or 0 if no object found.
    */
    ModelName* find(qint64 id)
    {
        QList<QSqlRecord> list;
        list.clear();
        try
        {
            list = DB.find(m_tableName, "*", getIdCondition(id));
        }
        catch (CExceptionSql *e)
        {
        }
        if(list.isEmpty())
            return 0;
        else
            return translateRecToObj<ModelName>(list.first());
    }

    void findThis(qint64 id)
    {
        QList<QSqlRecord> list;
        list.clear();
        try
        {
            list = DB.find(m_tableName, "*",  getIdCondition(id));
        }
        catch (CExceptionSql *e)
        {
        }
        if(!list.isEmpty())
            translateRecToThisObj(list.first());
    }

    /*!
       Finds all records in table.
       Returns list of found objects.
    */
    QList<ModelName*> findAll(const QString &orderby = QString())
    {
        QList<QSqlRecord> list;
        QString orderString = " ORDER BY ";
        setTableName();
        list.clear();
        try
        {
            list = DB.find(m_tableName, "*", orderby.isEmpty() ? QString() : orderString + orderby);
        }
        catch (CExceptionSql *e)
        {
        }
        QList<ModelName*> returnList;
        returnList.clear();
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }

    /*!
       Finds first object in table.
       Returns pointer to found model or 0 if no object found.
    */
    ModelName* first()
    {
        setTableName();
        QString pkName = QString("id_%1").arg(m_tableName);

        QSqlRecord record;
        record.clear();
        try
        {
            record = DB.first(m_tableName, pkName);
        }
        catch (CExceptionSql *e)
        {
        }
        if(record.isNull(QString("id_%1").arg(m_tableName)))
            return 0;
        else
            return translateRecToObj<ModelName>(record);
    }
    /*!
       Finds last object in table.
       Returns pointer to found model or 0 if no object found.
    */
    ModelName* last()
    {
        setTableName();
        QString pkName = QString("id_%1").arg(m_tableName);

        QSqlRecord record;
        record.clear();
        try
        {
            record = DB.last(m_tableName, pkName);
        }
        catch (CExceptionSql *e)
        {
        }
        if(record.isNull(QString("id_%1").arg(m_tableName)))
            return 0;
        else
            return translateRecToObj<ModelName>(record);
    }

    /*!
       Finds object by many fields and values.
       \a params - QHash<fieldName, value>, of which will be searched.
        Returns list of found objects.
    */
    /*!
       Finds objects by some field and value.
       Returns list of found objects.
    */
    QList<ModelName*> findBy(const QString &fieldName, const QVariant value, const QString &orderby = QString())
    {
        setTableName();
        QList<QSqlRecord> list;
        QString whereString = " WHERE ";
        QString orderString = orderby.isEmpty() ? "" : " ORDER BY " + orderby;

        whereString += QString(" %1 = '%2' ").arg(fieldName, value.toString());
        list.clear();
        try
        {
            list = DB.find(m_tableName, "*",  whereString + " " + orderString);
        }
        catch (CExceptionSql *e)
        {
        }
        QList<ModelName*> returnList;
        returnList.clear();
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }
    /*!
       Finds objects by vector of values.
       Returns list of found objects.
    */
    QList<ModelName*> findBy(const QString &fieldName, const QVector<QVariant> &values, const QString &orderby = QString())
    {
        QList<ModelName*> returnList;
        QString whereString = " WHERE ";
        QString orderString = orderby.isEmpty() ? "" : " ORDER BY " + orderby;

        returnList.clear();
        if(values.isEmpty())
            return returnList;

        Q_FOREACH(auto value, values)
            whereString += QString("%1 = '%2' OR ")
                               .arg(fieldName)
                               .arg(value.toString());
        whereString.chop(4);
        setTableName();

        QList<QSqlRecord> list;
        list.clear();
        try
        {
            list = DB.find(m_tableName, "*", whereString + " " + orderString);
        }
        catch (CExceptionSql *e)
        {
        }
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }

    QList<ModelName*> findByOr(const QHash<QString, QVariant> &params)
    {
        QList<ModelName*> returnList;

        returnList.clear();
        if(params.isEmpty())
            return returnList;

        QString whereString = "WHERE ";
        Q_FOREACH(auto key, params.keys())
            whereString += QString("%1 = '%2' OR ")
                               .arg(key)
                               .arg(params.value(key).toString());
        whereString.chop(4);
        setTableName();
        QList<QSqlRecord> list;
        list.clear();
        try
        {
            list = DB.find(m_tableName, "*", whereString);
        }
        catch (CExceptionSql *e)
        {
        }
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }
    /*!
       Finds object by many fields and values.
       \a params - QHash<fieldName, value>, direct will be searched.
        Returns list of found objects.
     */
    QList<ModelName*> findByAnd(const QHash<QString, QVariant> &params)
    {
        QList<ModelName*> returnList;

        returnList.clear();
        if(params.isEmpty())
            return returnList;

        QString whereString = "WHERE ";
        Q_FOREACH(auto key, params.keys())
            whereString += QString("%1 = '%2' AND ")
                               .arg(key)
                               .arg(params.value(key).toString());
        whereString.chop(4);
        setTableName();
        QList<QSqlRecord> list;
        list.clear();
        try
        {
            list = DB.find(m_tableName, "*", whereString);
        }
        catch (CExceptionSql *e)
        {
        }
        for(int i = 0; i < list.size(); i++)
            returnList.append(translateRecToObj<ModelName>(list.value(i)));
        return returnList;
    }

    /*!
       Returns true if table is empty, otherwise return false.
     */
    bool existsTable()
    {
        bool ret = true;
        setTableName();
        try
        {
            ret = !DB.find(m_tableName, "*", "").isEmpty();
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }
    /*!
       Returns true if object with given \a id exist, otherwise return false.
     */

    bool exists(qint64 id)
    {
        bool ret = false;
        setTableName();        
        try
        {
            ret = !DB.find(m_tableName, "*", getIdCondition(id)).isEmpty();
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    /*!
       Immediately updates object field in table.

       Returns true if property updated, otherwise return false.
     */
    bool updateProperty(const QString &fieldName, QVariant value)
    {
        if(m_id < 0)
            return false;
        QHash<QString, QVariant> info;
        info.insert(fieldName, value);
        setTableName();
        bool ret = false;
        try
        {
            if(DB.updateRecord(m_tableName, info, getIdCondition(m_id)))
            {
                setProperty(qPrintable(fieldName), value);
                m_propertiesForUpdate.removeAt(m_propertiesForUpdate.indexOf(fieldName));
                ret = true;
            }
        }
        catch (CExceptionSql *e)
        {            
        }
        return ret;

    }
    /*!
       Removes table's record with object's id.

       Returns true if success, otherwise return false.
     */
    bool remove()
    {
        bool ret = false;
        setTableName();
        try
        {
           ret = DB.remove(m_tableName, getIdCondition(m_id));
        }
        catch (CExceptionSql *e)
        {
        }
        if (ret)
            m_id = -1;

        return ret;
    }

    bool setDeleted()
    {
        bool ret = false;
        setTableName();
        try
        {
            ret = DB.setDeleted(m_tableName, getIdCondition(m_id));
        }
        catch (CExceptionSql *e)
        {
        }
        if (ret)
            m_id = -1;

        return ret;
    }

    /*!
       Removes all records from table.

       Returns true is success, otherwise return false.
     */
    bool removeAll() const
    {
        bool ret = false;
        try
        {
          ret = DB.remove(m_tableName, "");
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    bool deleteAll() const
    {
        bool ret = false;
        try
        {
            ret = DB.setDeleted(m_tableName, "");
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    /*!
       Returns number of objects in table.
     */
    qint64 count() const
    {
        qint64 ret = -1;
        try
        {
            ret =  DB.count(m_tableName, "*");
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }
    /*!
       Returns number of not null fields with given \a fieldName column.
     */
    qint64 count(const QString &fieldName) const
    {
        qint64 ret = -1;
        try
        {
            ret =  DB.count(m_tableName, fieldName);
        }
        catch (CExceptionSql *e)
        {
        }
        return ret;
    }

    PkNames getColumnFromModel(const QString &name)
    {
        PkNames field;
        field.first = QString("");
        field.second = QString("");
        Q_FOREACH (auto f, m_sqlDescriptions)
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

    QList<QSqlIndex> getTableIndexes()
    {
        QList<QSqlIndex> list;
        list.clear();
        try
        {
            list =  DB.getTableIndexes(m_tableName);
        }
        catch (CExceptionSql *e)
        {
        }
        return list;
    }

    QHash <QString, QString> getTableIndexesNames()
    {
        QHash <QString, QString> result;
        QList<QSqlIndex> idx_list = DB.getTableIndexes(m_tableName);
        Q_FOREACH(auto idx, idx_list)
        {
            result.insert(idx.cursorName(), idx.name());
        }
        return result;
    }

protected:
    bool m_hasUnsavedChanges;
    QStringList m_propertiesForUpdate;
    QString m_tableName;

    QList <PkNames> m_sqlDescriptions;
    QHash <QString, QString> m_tableTypes;
    QHash <QString, QString> m_indexFields;
    QHash <QString, QString> m_uniqueIndexFields;

    template<class T>
    T* translateRecToObj(const QSqlRecord &record) const
    {
        T *result = new T;
        QString currentFieldName;
        QString tableName = result->metaObject()->className();
        tableName = Tools::tableize(tableName);
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

    void translateRecToThisObj(const QSqlRecord &record)
    {
        QString currentFieldName;
        QString tableName = this->getTableName();

        for(int i = 1; i < this->metaObject()->propertyCount(); i++)
        {
            currentFieldName = QString(this->metaObject()->property(i).name());
            if(record.contains(currentFieldName))
                this->metaObject()->property(i).write(this, record.value(currentFieldName));
            else if(record.contains(currentFieldName.toLower()))
                this->metaObject()->property(i).write(this, record.value(currentFieldName.toLower()));
        }
        this->setId(record.value(QString("id_%1").arg(tableName)).toLongLong());
        //BASIC REC INFO
        this->set_deleted(record.value("deleted").toBool());
        this->set_timestp(record.value("timestp").toDateTime());
        this->clearUpdateList();
    }

};

#endif // CORMOBJECT_H
