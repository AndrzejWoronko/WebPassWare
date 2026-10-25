#ifndef PASSENTRYSERVICE_H
#define PASSENTRYSERVICE_H

#include "Global.h"
#include "SqlModel.h"
#include "OrmService.h"
#include "PassEntry.h"

class PassEntryService : public COrmService<PassEntry>
{
public:
    explicit PassEntryService(CDatabase &db) : COrmService(db) {}
    ~PassEntryService() = default;

    CSqlModel *getSqlModel(void);
};


#endif // PASSENTRYSERVICE_H
