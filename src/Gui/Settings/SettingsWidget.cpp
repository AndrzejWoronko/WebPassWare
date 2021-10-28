#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget (parent)
{
    setGraphicElements();
    setGraphicSettings();
    setConnections();
}

SettingsWidget::~SettingsWidget()
{
    safe_delete(m_look_widget)
    safe_delete(m_database_widget)

    safe_delete(m_list)
    safe_delete(m_layout)
    safe_delete(m_splitter)
    safe_delete(m_mainLayout)
}

void SettingsWidget::setGraphicElements(QWidget *parent)
{
    m_splitter = new CSplitter(QString("SettingsSplitter"), Qt::Horizontal, this);

    m_look_widget = new SettingsLookController(parent);
    m_database_widget = new SettingsDatabaseViewController(parent);
    this->setLayout(m_mainLayout = new CVBoxLayout());
    createListWidget();

    m_splitter->addWidget(m_list);
    m_layout = new QStackedLayout(m_splitter);
    m_layout->addWidget(m_look_widget->getView());
    m_layout->addWidget(m_database_widget->getView());
    m_splitter->setLayout(m_layout);
    m_mainLayout->addWidget(m_splitter);
}

void SettingsWidget::setGraphicSettings()
{
    m_list->setCurrentRow(0);
}

void SettingsWidget::createListWidget()
{
    m_list = new CListWidget(m_splitter);
    m_list->addItem(tr("Wygląd"), tr("Ustawienia wyglądu"), ICON("Settings"), QSize(50,50));
    m_list->addItem(tr("Baza danych"), tr("Parametry połączenia z bazą danych"), ICON("Database-settings"), QSize(50,50));
    m_list->setTabKeyNavigation(true);
//TODO WYMIAR LISTY Z KONFIGURACJI
    m_list->setMinimumWidth(200);
}

void SettingsWidget::setConnections()
{
    connect(m_list, SIGNAL(currentRowChanged(int)), m_layout, SLOT(setCurrentIndex(int)));
}
