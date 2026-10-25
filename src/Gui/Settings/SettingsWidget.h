#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "Global.h"
#include <QListWidget>
#include <QStackedLayout>
#include "Splitter.h"
#include "ListWidget.h"
#include "Layouts.h"
#include "SettingsLookController.h"
#include "SettingsDatabaseViewController.h"

class CApplication;
class CDatabase;

class SettingsWidget : public QWidget
{
    Q_OBJECT

    ADD_PTR_PROPERTY(SettingsLookController, m_look_widget, getLookWidget)
    ADD_PTR_PROPERTY(SettingsDatabaseViewController, m_database_widget, getDatabaseWidget)

    ADD_PTR_PROPERTY(QStackedLayout, m_layout, getLayout)
    ADD_PTR_PROPERTY(CListWidget, m_list, getList)
    ADD_PTR_PROPERTY(CVBoxLayout, m_mainLayout, getMainLayout)
    ADD_PTR_PROPERTY(CSplitter, m_splitter, getSplitter)

    void setGraphicElements(QWidget *parent = nullptr);
    void setGraphicSettings();
    void setConnections();

public:
    SettingsWidget(CApplication &app, CDatabase &db, QWidget *parent = nullptr);
    ~SettingsWidget();

    /**
  Metoda, która tworzy listę, której elementami są nazwy wszystkich modułów ustawień, stanowi ona
  żródło interakcji pomiędzy nimi
  @see
  createListWidgetItem()
 */
    void createListWidget();

private:
    CApplication &m_app;
    CDatabase &m_db;
};

#endif // SETTINGSWIDGET_H
