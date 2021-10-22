#include "PassGroupService.h"

PassGroupService::PassGroupService()
{

}

CSqlModel *PassGroupService::getSqlModel(void)
{
    CSqlModel *sql_model = NULL;
    SqlFieldsDesc list;

    list.clear();
    list.insert("id_pass_group", new CSqlField(QString("long"), tr("Id grupy"), tr("Id grupy"), list.count()));
    list.insert("m_name", new CSqlField(QString("string"), tr("Nazwa grupy"), tr("Nazwa grupy"), list.count()));

    QString query = CSqlModel::getSelectQueryStringFromFieldsDescList(list, QString("pass_group"));
    sql_model = new CSqlModel(query, QString("pass_group"), list);
    return sql_model;
}
