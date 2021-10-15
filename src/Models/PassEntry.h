#ifndef PASSENTRY_H
#define PASSENTRY_H

#include "Global.h"
#include "OrmObject.h"

class PassEntry : public COrmObject<PassEntry>
{
    Q_OBJECT

    ORM_PROPERTY(Varchar255, m_title)
    ORM_PROPERTY(QString, m_user)
    ORM_PROPERTY(QString, m_pass)
    ORM_PROPERTY(Varchar255, m_web_url)
    ORM_PROPERTY(Varchar255, m_desc)
    ORM_PROPERTY(qint64, m_id_pass_group)

    public:
    PassEntry()
    {
        initORMObject();

        ADD_VARCHAR("m_user", 52)
        ADD_VARCHAR("m_pass", 52)
        ADD_SQL_INDEX("m_name", "m_name")
        ADD_SQL_INDEX("m_id_pass_group", "m_id_pass_group")
    }
};

#endif // PASSENTRY_H
