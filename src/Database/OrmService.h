#ifndef CORMSERVICE_H
#define CORMSERVICE_H

#include "Global.h"
#include "OrmObject.h"
#include "Service.h"

template<class ModelName>
class COrmService : public CService, public COrmObject<ModelName>
{

    ModelName *m_object;
    qint64 m_insertedId;

public:
    COrmService()
    {
        m_insertedId = -1;
        m_object = new ModelName;
    }

    virtual ~COrmService()
    {
        DEBUG_WITH_LINE << "OrmService destructor";
        delete m_object;
    }

    ModelName* getObject(void)
    {
        return m_object;
    }

    qint64 getInsertedId()
    {
        return m_insertedId;
    }

    QList<ModelName*> getObjects(const QString &orderby = QString())
    {
        QList<ModelName*> list;
        try
        {
            list = m_object->findAll(orderby);

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
        return list;
    }

    QList<ModelName*> getObjectsBy(const QString &fieldName, const QVariant value)
    {
        QList<ModelName*> list;
        try
        {
            list = m_object->findBy(fieldName, value);

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
        return list;
    }

    QList<ModelName*> getObjectsBy(const QHash<QString, QVariant> &params)
    {
        QList<ModelName*> list;
        try
        {
            list = m_object->findByAnd(params);

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
        return list;
    }

    ModelName *getFirstObject()
    {
        ModelName* o = NULL;
        try
        {
            o = m_object->first();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
        return o;
    }

    ModelName* getObject(qint64 id)
    {
        ModelName *o = NULL;
        try
        {
            o = m_object->find(id);

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
        return o;
    }

    qint64 addObject(ModelName *o, qint64 newId = 0)
    {
        try
        {
            o->save(newId);
            m_insertedId = o->getId();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
        return m_insertedId;
    }

    bool editObject(ModelName *o)
    {
        bool ret = false;
        try
        {
            ret = o->update();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
        return ret;
    }

    void removeObject(qint64 id)
    {
        try
        {
            ModelName *o = m_object->find(id);
            if (o)
                o->remove();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
    }

    void deleteObject(qint64 id)
    {
        try
        {
            ModelName *o = m_object->find(id);
            if (o)
                o->setDeleted();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
        }
    }
};

#endif // CORMSERVICE_H
