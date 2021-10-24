#include "PassEntryDialog.h"

PassEntryDialog::PassEntryDialog(QWidget *parent) :
    CAbstractFormDialog(QString("PASS_ENTRY_DIALOG"), parent)
{

}

void PassEntryDialog::addFields()
{
    m_id = new CFormNumberField(QString("m_id"), tr("Id"), 0);
    addField(m_id);
    m_title = new CFormTextField(QString("m_title"), tr("Tytuł"), "", 255);
    addField(m_title);
    m_user = new CFormTextField(QString("m_user"), tr("Użytkownik"), "", 52);
    addField(m_user);
    m_pass = new CFormTextField(QString("m_pass"), tr("Hasło"), "", 52);
    addField(m_pass);
    m_web_url = new CFormTextField(QString("m_web_url"), tr("Adres strony WWW"), "", 255);
    addField(m_web_url);
    m_desc = new CFormTextField(QString("m_desc"), tr("Opis"), "", 255);
    addField(m_desc);
    m_id_pass_group = new CFormSimpleIndexChoiceField(QString("m_id_pass_group"), tr("Grupa"), "");
    addField(m_id_pass_group);
}

void PassEntryDialog::addComponents()
{
    CForm::setComponents(getFormLayout(), m_id, m_id->getLabel(), 0, 1, 1, 1, true);
    CForm::setComponents(getFormLayout(), m_title, m_title->getLabel(), 1, 1, 1, 2);
    CForm::setComponents(getFormLayout(), m_user, m_user->getLabel(), 2, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_pass, m_pass->getLabel(), 3, 1, 1, 1);
    CForm::setComponents(getFormLayout(), m_web_url, m_web_url->getLabel(), 4, 1, 1, 2);
    CForm::setComponents(getFormLayout(), m_desc, m_desc->getLabel(), 5, 1, 1, 3);
    CForm::setComponents(getFormLayout(), m_id_pass_group, m_id_pass_group->getLabel(), 6, 1, 1, 2);
    setDialogWidget();
}
