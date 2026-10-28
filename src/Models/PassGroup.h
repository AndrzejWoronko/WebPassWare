#ifndef PASSGROUP_H
#define PASSGROUP_H

#include "Global.h"
#include "OrmObject.h"

namespace wpw {

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

} // namespace wpw

#endif // PASSGROUP_H
