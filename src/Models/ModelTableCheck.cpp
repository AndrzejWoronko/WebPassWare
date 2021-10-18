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

    PassEntry pe;
    checkOneTable(&pe);
}
