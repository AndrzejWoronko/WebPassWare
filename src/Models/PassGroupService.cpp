#include "PassGroupService.h"

CSqlModel* PassGroupService::getSqlModel(void)
{    
    SqlFieldsDesc list;

    list.clear();
    list.insert("id_pass_group", new CSqlField(QString("long"), tr("Id grupy"), tr("Id grupy"), list.count()));
    list.insert("m_name", new CSqlField(QString("string"), tr("Nazwa grupy"), tr("Nazwa grupy"), list.count()));

    QString query = CSqlModel::getSelectQueryStringFromFieldsDescList(list, QString("pass_group"));
    QScopedPointer<CSqlModel> sql_model(new CSqlModel(query, QString("pass_group"), list));
    return sql_model.take();
}

QStringList PassGroupService::getGroupNameList(void)
{
    QList<PassGroup*> list = this->getObjects(QString("id_pass_group"));
    QStringList ret_list;
    Q_FOREACH(auto pg, list)
    {
        ret_list.append(pg->getm_name());
    }
    return ret_list;
}
