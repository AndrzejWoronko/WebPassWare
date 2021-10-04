#include "ButtonBox.h"
#include "Style.h"

CButtonBox::CButtonBox(QWidget *parent) : QDialogButtonBox(parent)
{
    m_cancel_button = this->addButton(tr("&Anuluj"), QDialogButtonBox::RejectRole);
    m_save_button = this->addButton(tr("&Zapisz"), QDialogButtonBox::AcceptRole);

    m_cancel_button->setStatusTip(tr("Anuluj"));
    m_cancel_button->setToolTip(tr("Anuluj"));

    m_save_button->setStatusTip(tr("Zapisz"));
    m_save_button->setToolTip(tr("Zapisz"));

    this->setMinimumSize(25, 25);

    if (CStyle::iconFromStyleExist(ICON("Cancel")))
        m_cancel_button->setIcon(CStyle::iconFromStyle(ICON("Cancel")));
    else
        m_cancel_button->setIcon(QIcon::fromTheme("cancel"));

    if (CStyle::iconFromStyleExist(ICON("Save")))
        m_save_button->setIcon(CStyle::iconFromStyle(ICON("Save")));
    else
        m_save_button->setIcon(QIcon::fromTheme("save"));
}

CButtonBoxSave::CButtonBoxSave(QWidget *parent) : CButtonBox(parent)
{

}

CButtonBoxChoice::CButtonBoxChoice(QWidget *parent) : CButtonBox(parent)
{
    m_save_button->setText(tr("&Wybierz"));
    m_save_button->setStatusTip(tr("Wybierz"));
    m_save_button->setToolTip(tr("Wybierz"));

    if (CStyle::iconFromStyleExist(ICON("Select")))
        m_save_button->setIcon(CStyle::iconFromStyle(ICON("Select")));
    else
        m_save_button->setIcon(QIcon::fromTheme("choice"));
}

CButtonBoxExport::CButtonBoxExport(QWidget *parent) : CButtonBox(parent)
{
    m_save_button->setText(tr("&Export"));
    m_save_button->setStatusTip(tr("Export"));
    m_save_button->setToolTip(tr("Export"));

    if (CStyle::iconFromStyleExist(ICON("Export")))
        m_save_button->setIcon(CStyle::iconFromStyle(ICON("Export")));
    else
        m_save_button->setIcon(QIcon::fromTheme("export"));
}

CButtonBoxOk::CButtonBoxOk(QWidget *parent) : CButtonBox(parent)
{
    m_save_button->setText(tr("&Ok"));
    m_save_button->setStatusTip(tr("Ok"));
    m_save_button->setToolTip(tr("Ok"));

    if (CStyle::iconFromStyleExist(ICON("Ok")))
        m_save_button->setIcon(CStyle::iconFromStyle(ICON("Ok")));
    else
        m_save_button->setIcon(QIcon::fromTheme("ok"));
}


CButtonBoxApplyRestore::CButtonBoxApplyRestore(QWidget *parent) : CButtonBox(parent)
{
    m_restore_button = this->addButton(tr("&Przywróć"), QDialogButtonBox::ResetRole);
    m_restore_button->setStatusTip(tr("Przywróć ustawienia"));
    m_restore_button->setToolTip(tr("Przywróć ustawienia"));

    m_save_button->setText(tr("&Zastosuj"));
    m_save_button->setStatusTip(tr("Zastosuj"));
    m_save_button->setToolTip(tr("Zastosuj"));

    if (CStyle::iconFromStyleExist(ICON("Apply")))
        m_save_button->setIcon(CStyle::iconFromStyle(ICON("Apply")));
    else
        m_save_button->setIcon(QIcon::fromTheme("apply"));

    if (CStyle::iconFromStyleExist(ICON("Restore")))
        m_restore_button->setIcon(CStyle::iconFromStyle(ICON("Restore")));
    else
        m_restore_button->setIcon(QIcon::fromTheme("restore"));
}

CButtonBoxClose::CButtonBoxClose(QWidget *parent) : QDialogButtonBox(parent)
{
    m_close_button = this->addButton(tr("&Zamknij"), QDialogButtonBox::RejectRole);

    m_close_button->setStatusTip(tr("Zamknij"));
    m_close_button->setToolTip(tr("Zamknij"));
    this->setMinimumSize(25, 25);

    if (CStyle::iconFromStyleExist(ICON("Close")))
        m_close_button->setIcon(CStyle::iconFromStyle(ICON("Close")));
    else
        m_close_button->setIcon(QIcon::fromTheme("close"));
}
