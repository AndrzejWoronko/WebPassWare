#include "AbstractFormWidget.h"

CAbstractFormWidget::CAbstractFormWidget()
{

}

CAbstractFormWidget::CAbstractFormWidget(QWidget *parent)
    : QWidget(parent)
{
    m_fields.clear();
    m_formLayout  = new CGridLayout(this);
    //TODO Ustawić z konfiguracji minmalną wielkość widgetów
    this->setMinimumSize(320, 240);
}

CAbstractFormWidget::~CAbstractFormWidget()
{
    safe_delete(m_formLayout)
}

void CAbstractFormWidget::addFieldsAndComponents()
{
    addFields();
    addComponents();
}

FormFieldHashList CAbstractFormWidget::getFields()
{
    return m_fields;
}

void CAbstractFormWidget::addField(CFormAbstractField *f)
{
    m_fields.insert(f->getVariableName(), f);
}

CAbstractFormWidget *CAbstractFormWidget::getWidget()
{
    return this;
}
