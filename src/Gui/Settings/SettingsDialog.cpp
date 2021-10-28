#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *w, QWidget *parent) : CDialog(parent), CAbstractDialogStateManager()
{
    this->setWindowTitle(tr("Ustawienia"));
    m_widget = w;
    addFieldsAndComponents();
    //TODO Ustawić z konfiguracji minmalną wielkość dialogów
    this->setMinimumSize(640, 480);
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


    //connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    m_widget->setFocus();
    this->restoreDialogState();
}

SettingsDialog::~SettingsDialog()
{
    this->saveDialogState();
    //    delete m_VLayoutDialog;
    //    delete m_buttonBox;
}

void SettingsDialog::restoreDialogState()
{
    QByteArray state = m_dialogState->getState(QString("SETTINGS_DIALOG"));
    this->restoreGeometry(state);
}

void SettingsDialog::saveDialogState()
{
    QByteArray state = this->saveGeometry();
    QByteArray oldState = m_dialogState->getState(QString("SETTINGS_DIALOG"));

    if(state != oldState)
        m_dialogState->saveState(QString("SETTINGS_DIALOG"), state);
}
