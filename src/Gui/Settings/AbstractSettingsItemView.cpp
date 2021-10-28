#include "AbstractSettingsItemView.h"

CAbstractSettingsItemView::CAbstractSettingsItemView(const QString &title, QWidget *parent) : QWidget (parent)
{
    m_fields.clear();
    m_VLayoutWidget = new CVBoxLayout(this);

    //m_scrollArea = new QScrollArea(this);
    //m_scrollArea->setWidgetResizable(true);

    m_widget = new QWidget();
    m_formLayout  = new CGridLayout(m_widget);

    //m_scrollArea->setWidget(m_widget);

    m_title_label = new CLabel(title, QString(), this);
    m_title_label->setBold();

    m_button_box = new CButtonBoxApplyRestore(this);
    //Przy wejściu klawisze Zastosuj i anulij nie aktywne
    m_button_box->getSaveButton()->setEnabled(false);
    m_button_box->getCancelButton()->setEnabled(false);

    //TODO Ustawić z konfiguracji minmalną wielkość widgetów
    this->setMinimumSize(320, 240);
}

CAbstractSettingsItemView::~CAbstractSettingsItemView()
{
    safe_delete(m_title_label)
    safe_delete(m_formLayout)
    safe_delete(m_widget)
    //safe_delete(m_scrollArea)
}

void CAbstractSettingsItemView::addFieldsAndComponents()
{
    m_VLayoutWidget->addWidget(m_title_label);
    addFields();
    addComponents();
    //m_VLayoutWidget->addWidget(m_scrollArea);
    m_VLayoutWidget->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_VLayoutWidget->addWidget(m_button_box);
}

void CAbstractSettingsItemView::setWidgetWidget(void)
{
    m_VLayoutWidget->addWidget(m_widget);
}

void CAbstractSettingsItemView::setWidgetWidget(QTabWidget *tabWidget)
{
    m_VLayoutWidget->addWidget(tabWidget);
}

void CAbstractSettingsItemView::setWidgetWidget(QWidget *widget)
{
    m_VLayoutWidget->addWidget(widget);
}

FormFieldHashList CAbstractSettingsItemView::getFields()
{
    return m_fields;
}

void CAbstractSettingsItemView::addField(CFormAbstractField *f)
{
    m_fields.insert(f->getVariableName(), f);
}

void CAbstractSettingsItemView::emitChangesWereMade()
{
    emit changesWereMade();
}
