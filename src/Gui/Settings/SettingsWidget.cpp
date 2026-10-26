#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(CApplication &app, CDatabase &db, QWidget *parent) : QWidget (parent), m_app(app), m_db(db)
{
    setGraphicElements();
    setGraphicSettings();
    setConnections();
}

SettingsWidget::~SettingsWidget()
{
    // The controllers delete their pages.
    m_pages.clear();

    safe_delete(m_list);
    safe_delete(m_layout);
    safe_delete(m_splitter);
    safe_delete(m_mainLayout);
}

void SettingsWidget::setGraphicElements()
{
    m_splitter = new CSplitter(QString("SettingsSplitter"), Qt::Horizontal, this);

    this->setLayout(m_mainLayout = new CVBoxLayout());
    createListWidget();

    m_splitter->addWidget(m_list);
    m_layout = new QStackedLayout(m_splitter);

    addPage(tr("Wygląd"), tr("Ustawienia wyglądu"), ICON("Settings"), std::make_unique<SettingsLookController>(m_app));
    addPage(tr("Baza danych"), tr("Parametry połączenia z bazą danych"), ICON("Database-settings"),
            std::make_unique<SettingsDatabaseViewController>(m_db));

    m_splitter->setLayout(m_layout);
    m_mainLayout->addWidget(m_splitter);
}

void SettingsWidget::addPage(const QString &title, const QString &description, const QString &iconName,
                             std::unique_ptr<CAbstractSettingsItemController> controller)
{
    m_list->addItem(title, description, iconName, QSize(50,50));
    m_layout->addWidget(controller->getView());
    m_pages.push_back(std::move(controller));
}

void SettingsWidget::setGraphicSettings()
{
    m_list->setCurrentRow(0);
}

void SettingsWidget::createListWidget()
{
    m_list = new CListWidget(m_splitter);
    m_list->setTabKeyNavigation(true);
//TODO WYMIAR LISTY Z KONFIGURACJI
    m_list->setMinimumWidth(200);
}

void SettingsWidget::setConnections()
{
    connect(m_list, &CListWidget::currentRowChanged, m_layout, &QStackedLayout::setCurrentIndex);
}
