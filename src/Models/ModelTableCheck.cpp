#include "ModelTableCheck.h"
#include "PassEntry.h"
#include "PassGroup.h"


ModelTableCheck::ModelTableCheck()
{

}

void ModelTableCheck::checkAllTables()
{
    PassGroup pg;
    checkOneTable(&pg);
    if (pg.count() == 0)
       {
          DB.resetAutoIncrement(pg.getTableName(), 0);
          pg.setId(0);
          pg.setm_name(QObject::tr("Wszystkie grupy"));
          pg.save(0, QString(), true);
       }
    PassEntry pe;
    checkOneTable(&pe);
}
