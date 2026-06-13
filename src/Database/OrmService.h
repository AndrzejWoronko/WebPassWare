#ifndef CORMSERVICE_H
#define CORMSERVICE_H

#include "Global.h"
#include "OrmRepository.h"
#include "Service.h"

template<class ModelName>
class COrmService
{

    std::unique_ptr<IRepository<ModelName>> m_repository;
    qint64 m_insertedId;
    std::unique_ptr<ICService> m_service;

public:
    COrmService()
    {
        m_insertedId = -1;
        m_repository = std::make_unique<COrmRepository<ModelName>>();
        m_service = std::make_unique<CService>();
    }

    virtual ~COrmService()
    {
        DEBUG_WITH_LINE << "OrmService destructor";
    }

    ModelName* getObject(void)
    {
        return m_repository->prototype();
    }

    qint64 getInsertedId()
    {
        return m_insertedId;
    }

    QList<ModelName*> getObjects(const QString &orderby = QString())
    {
        QList<ModelName*> list;
        m_service->clearError();
        try
        {
            list = m_repository->findAll(orderby);

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return list;
    }

    QList<ModelName*> getObjectsBy(const QString &fieldName, const QVariant value)
    {
        QList<ModelName*> list;
        m_service->clearError();
        try
        {
            list = m_repository->findBy(fieldName, value);

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return list;
    }

    QList<ModelName*> getObjectsBy(const QHash<QString, QVariant> &params)
    {
        QList<ModelName*> list;
        m_service->clearError();
        try
        {
            list = m_repository->findByAnd(params);

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return list;
    }

    ModelName *getFirstObject()
    {
        ModelName* o = nullptr;
        m_service->clearError();
        try
        {
            o = m_repository->first();

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return o;
    }

    ModelName* getObject(qint64 id)
    {
        ModelName *o = nullptr;
        m_service->clearError();
        try
        {
            o = m_repository->find(id);

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return o;
    }

    qint64 addObject(ModelName *o, qint64 newId = 0)
    {
        m_service->clearError();
        try
        {
            m_insertedId = m_repository->add(o, newId);

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return m_insertedId;
    }

    bool editObject(ModelName *o)
    {
        bool ret = false;
        m_service->clearError();
        try
        {
            ret = m_repository->update(o);

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return ret;
    }

    bool removeObject(qint64 id)
    {
        bool ret = false;
        m_service->clearError();
        try
        {
            ret = m_repository->remove(id);

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return ret;
    }

    bool deleteObject(qint64 id)
    {
        bool ret = false;
        m_service->clearError();
        try
        {
            ret = m_repository->markDeleted(id);

        } catch(CExceptionSql &e)
        {
            m_service->showErrorInfo(e);
            m_service->setError(e.getMessage());
        }
        return ret;
    }

    QString getError()
    {
        return m_service->getError();
    }
};

#endif // CORMSERVICE_H
