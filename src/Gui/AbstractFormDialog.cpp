#include "AbstractFormDialog.h"

CAbstractFormDialog::CAbstractFormDialog(const QString &dialog_name, QWidget *parent)
    : CDialog(parent) , m_dialog_name(dialog_name)
{
    m_fields.clear();
    m_VLayoutDialog = new CVBoxLayout(this);
    m_widget = new QWidget();
    m_formLayout  = new CGridLayout(m_widget);
    //TODO Ustawić z konfiguracji minmalną wielkość dialogów
    //this->setMinimumSize(640, 480);
    this->setMinimumSize(320, 240);
}

CAbstractFormDialog::~CAbstractFormDialog()
{
    safe_delete(m_formLayout)
    safe_delete(m_widget)
}

void CAbstractFormDialog::addFieldsAndComponents()
{
    addFields();
    addComponents();
    createButtons();
}

void CAbstractFormDialog::setDialogWidget()
{
    m_VLayoutDialog->addWidget(m_widget);
}

void CAbstractFormDialog::setDialogWidget(QTabWidget *tabWidget)
{
    m_VLayoutDialog->addWidget(tabWidget);
}

void CAbstractFormDialog::setDialogWidget(QWidget *widget)
{
    m_VLayoutDialog->addWidget(widget);
}

void CAbstractFormDialog::createButtons()
{
    m_buttonBox = new CButtonBox(this);
    m_VLayoutDialog->addWidget(m_buttonBox);
}

void CAbstractFormDialog::closeEvent(QCloseEvent *event)
{
    if (event->type() == QEvent::Close)
    {
//        if (m_controller)
//            m_controller->checkChanges();
        event->accept();
    }
}

FormFieldHashList &CAbstractFormDialog::getFields()
{
    return m_fields;
}

void CAbstractFormDialog::addField(CFormAbstractField *f)
{
    m_fields.insert(f->getVariableName(), f);
}
