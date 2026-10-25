#ifndef PASSGROUPSERVICE_H
#define PASSGROUPSERVICE_H

#include "Global.h"
#include "SqlModel.h"
#include "OrmService.h"
#include "PassGroup.h"

// Emits changed() after every successful change, so views can refresh.
class PassGroupService : public QObject, public COrmService<PassGroup>
{
    Q_OBJECT

public:
    explicit PassGroupService(CDatabase &db, QObject *parent = nullptr) : QObject(parent), COrmService(db) {}

    void notifyChanged() override
    {
        emit changed();
    }

signals:
    void changed();

public:
    ~PassGroupService() = default;

    CSqlModel *getSqlModel(void);
    QStringList getGroupNameList(void);
    // (id, name) of the groups that are not deleted, by id.
    QList<QPair<qint64, QString>> getGroupChoices(void);
};

#endif // PASSGROUPSERVICE_H
