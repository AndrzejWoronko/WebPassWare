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
        safe_delete(m_object)
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
        this->clearError();
        try
        {
            list = m_object->findAll(orderby);

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return list;
    }

    QList<ModelName*> getObjectsBy(const QString &fieldName, const QVariant value)
    {
        QList<ModelName*> list;
        this->clearError();
        try
        {
            list = m_object->findBy(fieldName, value);

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return list;
    }

    QList<ModelName*> getObjectsBy(const QHash<QString, QVariant> &params)
    {
        QList<ModelName*> list;
        this->clearError();
        try
        {
            list = m_object->findByAnd(params);

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return list;
    }

    ModelName *getFirstObject()
    {
        ModelName* o = NULL;
        this->clearError();
        try
        {
            o = m_object->first();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return o;
    }

    ModelName* getObject(qint64 id)
    {
        ModelName *o = NULL;
        this->clearError();
        try
        {
            o = m_object->find(id);

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return o;
    }

    qint64 addObject(ModelName *o, qint64 newId = 0)
    {
        this->clearError();
        try
        {
            o->save(newId);
            m_insertedId = o->getId();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return m_insertedId;
    }

    bool editObject(ModelName *o)
    {
        bool ret = false;
        this->clearError();
        try
        {
            ret = o->update();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return ret;
    }

    bool removeObject(qint64 id)
    {
        bool ret = false;
        this->clearError();
        try
        {
            ModelName *o = m_object->find(id);
            if (o)
                ret = o->remove();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return ret;
    }

    bool deleteObject(qint64 id)
    {
        bool ret = false;
        this->clearError();
        try
        {
            ModelName *o = m_object->find(id);
            if (o)
                ret = o->setDeleted();

        } catch(CExceptionSql *e)
        {
            this->showErrorInfo(e);
            this->setError(e->getMessage());
        }
        return ret;
    }
};

#endif // CORMSERVICE_H
