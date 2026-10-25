#ifndef PASSENTRYSERVICE_H
#define PASSENTRYSERVICE_H

#include "Global.h"
#include "SqlModel.h"
#include "OrmService.h"
#include "PassEntry.h"

// Emits changed() after every successful change, so views can refresh.
class PassEntryService : public QObject, public COrmService<PassEntry>
{
    Q_OBJECT

public:
    explicit PassEntryService(CDatabase &db, QObject *parent = nullptr) : QObject(parent), COrmService(db) {}

    void notifyChanged() override
    {
        emit changed();
    }

signals:
    void changed();

public:
    ~PassEntryService() = default;

    CSqlModel *getSqlModel(void);
};


#endif // PASSENTRYSERVICE_H
