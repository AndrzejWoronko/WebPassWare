#include "ModelTableCheck.h"
#include "PassEntry.h"
#include "PassGroup.h"


ModelTableCheck::ModelTableCheck()
{

}

void ModelTableCheck::checkAllTables(CDatabase &db)
{
    COrmRepository<PassGroup> groups(db);
    checkOneTable(groups);
    if (groups.count() == 0 || !groups.exists(0))
       {
          PassGroup pg;
          db.resetAutoIncrement(pg.getTableName(), 0);
          pg.setId(0);
          pg.setm_name(QObject::tr("Wszystkie grupy"));
          groups.save(&pg, 0, true);
       }
    COrmRepository<PassEntry> entries(db);
    checkOneTable(entries);
}
