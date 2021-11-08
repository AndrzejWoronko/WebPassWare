#include "AbstractCloseDialog.h"


CAbstractCloseDialog::CAbstractCloseDialog(const QString &dialogName, QWidget *parent)
    : CDialog(parent),  CAbstractDialogStateManager()
{
      m_VLayoutDialog = new CVBoxLayout(this);
      m_widget = new QWidget();
      m_formLayout  = new CGridLayout(m_widget);
      setDialogName(dialogName);
}

CAbstractCloseDialog::~CAbstractCloseDialog()
{
    saveDialogState();
    safe_delete(m_formLayout)
    safe_delete(m_widget)
}

void CAbstractCloseDialog::addFieldsAndComponents()
{
    addFields();
    addComponents();
    createButtons();
    restoreDialogState();
}

void CAbstractCloseDialog::setDialogWidget(void)
{
    m_VLayoutDialog->addWidget(m_widget);
}

void CAbstractCloseDialog::setDialogWidget(QTabWidget *tabWidget)
{
    m_VLayoutDialog->addWidget(tabWidget);
}

void CAbstractCloseDialog::setDialogWidget(QWidget *widget)
{
    m_VLayoutDialog->addWidget(widget);
}

void CAbstractCloseDialog::createButtons()
{
    m_buttonBoxClose = new CButtonBoxClose(this);
    connect(m_buttonBoxClose, SIGNAL(rejected()), this, SLOT(reject()));
    m_VLayoutDialog->addWidget(m_buttonBoxClose);
}

//void CAbstractCloseDialog::closeEvent(QCloseEvent *event)
//{
//     if (event->type() == QEvent::Close)
//        {
//            event->accept();
//        }
//}

void CAbstractCloseDialog::restoreDialogState()
{
    QByteArray state = m_dialogState->getState(getDialogName());
    this->restoreGeometry(state);
}

void CAbstractCloseDialog::saveDialogState()
{
    QByteArray state = this->saveGeometry();
    QByteArray oldState = m_dialogState->getState(getDialogName());

    if(state != oldState)
        m_dialogState->saveState(getDialogName(), state);
}
