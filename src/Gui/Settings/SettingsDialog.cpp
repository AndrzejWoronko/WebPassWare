#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *w, QWidget *parent) : CDialog(parent), CAbstractDialogStateManager()
{
    this->setWindowTitle(tr("Ustawienia"));
    m_widget = w;
    addFieldsAndComponents();
}

void SettingsDialog::addFieldsAndComponents()
{
    m_VLayoutDialog = new CVBoxLayout(this);
    //m_widget->setMinimumSize(800,600);
    //m_widget->setFixedSize(800, 600);
    //m_widget->(800,600);

    m_VLayoutDialog->addWidget(m_widget);
    m_buttonBox = new CButtonBoxClose(this);
    m_VLayoutDialog->addWidget(m_buttonBox);


    //connect(m_buttonBox, &CButtonBoxClose::accepted, this, &SettingsDialog::accept);
    connect(m_buttonBox, &CButtonBoxClose::rejected, this, &SettingsDialog::reject);
    m_widget->setFocus();
    this->restoreDialogState();
}

SettingsDialog::~SettingsDialog()
{
    this->saveDialogState();
    //    safe_delete(m_VLayoutDialog)
    //    safe_delete(m_buttonBox)
}

void SettingsDialog::restoreDialogState()
{
    QByteArray state = getDialogState()->getState(QString("SETTINGS_DIALOG"));
    this->restoreGeometry(state);
}

void SettingsDialog::saveDialogState()
{
    QByteArray state = this->saveGeometry();
    QByteArray oldState = getDialogState()->getState(QString("SETTINGS_DIALOG"));

    if(state != oldState)
        getDialogState()->saveState(QString("SETTINGS_DIALOG"), state);
}
