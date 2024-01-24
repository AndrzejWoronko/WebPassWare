#include "PassEntryService.h"

CSqlModel *PassEntryService::getSqlModel(void)
{    
    SqlFieldsDesc list;

    list.clear();
    list.insert("id_pass_entry", new CSqlField(QString("long"), tr("Id"), tr("Id"), list.count()));
    list.insert("m_title", new CSqlField(QString("string"), tr("Tytuł"), tr("Tytuł rekordu"), list.count()));
    list.insert("m_user", new CSqlField(QString("string"), tr("Użytkownik"), tr("Nazwa użytkownika" ), list.count()));
    list.insert("m_pass", new CSqlField(QString("string"), tr("Hasło"), tr("Hasło użytkownika" ), list.count()));
    list.insert("m_web_url", new CSqlField(QString("string"), tr("Strona WWW"), tr("Strona WWW" ), list.count()));
    list.insert("pass_group_m_name", new CSqlField(QString("string"), tr("Grupa"), tr("Nazwa grupa rekordów" ), list.count(), 0, -1, 0,
    QString("COALESCE((SELECT m_name FROM pass_group WHERE m_id_pass_group = id_pass_group), 'Brak')")));
    list.insert("m_desc", new CSqlField(QString("string"), tr("Opis"), tr("Opis" ), list.count()));

    QString query = CSqlModel::getSelectQueryStringFromFieldsDescList(list, QString("pass_entry"));
    QScopedPointer<CSqlModel> sql_model(new CSqlModel(query, QString("pass_entry"), list));
    return sql_model.take();
}
