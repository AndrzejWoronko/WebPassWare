#ifndef PASSGROUP_H
#define PASSGROUP_H

#include "Global.h"
#include "OrmObject.h"

class PassGroup : public COrmObject<PassGroup>
{
    Q_OBJECT
    ORM_PROPERTY(QString, m_name)

public:
    PassGroup()
    {
        initORMObject();
        ADD_VARCHAR("m_name", 52)
        ADD_SQL_INDEX("m_name", "m_name")
    }
};

#endif // PASSGROUP_H
