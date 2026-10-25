#ifndef PASSGROUPSERVICE_H
#define PASSGROUPSERVICE_H

#include "Global.h"
#include "SqlModel.h"
#include "OrmService.h"
#include "PassGroup.h"

class PassGroupService : public COrmService<PassGroup>
{
public:
    explicit PassGroupService(CDatabase &db) : COrmService(db) {}
    ~PassGroupService() = default;

    CSqlModel *getSqlModel(void);
    QStringList getGroupNameList(void);
};

#endif // PASSGROUPSERVICE_H
