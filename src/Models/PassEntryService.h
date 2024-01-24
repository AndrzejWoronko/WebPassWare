#ifndef PASSENTRYSERVICE_H
#define PASSENTRYSERVICE_H

#include "Global.h"
#include "SqlModel.h"
#include "OrmService.h"
#include "PassEntry.h"

class PassEntryService : public COrmService<PassEntry>, public Singleton<PassEntryService>
{
public:
    PassEntryService() = default;
    ~PassEntryService() = default;

    CSqlModel *getSqlModel(void);
};


#endif // PASSENTRYSERVICE_H
