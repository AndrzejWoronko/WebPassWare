#include "AbstractFieldsDialog.h"

CAbstractFieldsDialog::CAbstractFieldsDialog(const QString &dialogName, QWidget *parent)
    : CDialog(parent), m_dialog_name(dialogName)
{
    m_VLayoutDialog = new CVBoxLayout(this);
    m_widget = new QWidget();
    m_formLayout = new CGridLayout(m_widget);
}

CAbstractFieldsDialog::~CAbstractFieldsDialog()
{
    // m_widget has no parent until setDialogWidget() places it.
    safe_delete(m_formLayout);
    safe_delete(m_widget);
}

void CAbstractFieldsDialog::addFieldsAndComponents()
{
    addFields();
    addComponents();
    createButtons();
}

void CAbstractFieldsDialog::setDialogWidget(void)
{
    m_VLayoutDialog->addWidget(m_widget);
}

void CAbstractFieldsDialog::setDialogWidget(QTabWidget *tabWidget)
{
    m_VLayoutDialog->addWidget(tabWidget);
}

void CAbstractFieldsDialog::setDialogWidget(QWidget *widget)
{
    m_VLayoutDialog->addWidget(widget);
}

CVBoxLayout *CAbstractFieldsDialog::dialogLayout()
{
    return m_VLayoutDialog;
}
