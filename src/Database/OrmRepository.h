#ifndef CORMREPOSITORY_H
#define CORMREPOSITORY_H

#include "Global.h"
#include "OrmObject.h"

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

template<class ModelName>
class COrmRepository : public IRepository<ModelName>
{
    std::unique_ptr<ModelName> m_object;

public:
    COrmRepository()
    {
        m_object = std::make_unique<ModelName>();
    }

    ModelName* prototype() override final
    {
        return m_object.get();
    }

    QList<ModelName*> findAll(const QString &orderby = QString()) override final
    {
        return m_object->findAll(orderby);
    }

    QList<ModelName*> findBy(const QString &fieldName, const QVariant value) override final
    {
        return m_object->findBy(fieldName, value);
    }

    QList<ModelName*> findByAnd(const QHash<QString, QVariant> &params) override final
    {
        return m_object->findByAnd(params);
    }

    ModelName* first() override final
    {
        return m_object->first();
    }

    ModelName* find(qint64 id) override final
    {
        return m_object->find(id);
    }

    qint64 add(ModelName *object, qint64 newId = 0) override final
    {
        object->save(newId);
        return object->getId();
    }

    bool update(ModelName *object) override final
    {
        return object->update();
    }

    bool remove(qint64 id) override final
    {
        QScopedPointer<ModelName> object(m_object->find(id));
        if (!object.isNull())
        {
            return object->remove();
        }
        return false;
    }

    bool markDeleted(qint64 id) override final
    {
        QScopedPointer<ModelName> object(m_object->find(id));
        if (!object.isNull())
        {
            return object->setDeleted();
        }
        return false;
    }
};

#endif // CORMREPOSITORY_H
