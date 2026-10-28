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

#include <memory>
#include <vector>

namespace wpw {

class CApplication;
class CDatabase;

class SettingsWidget : public QWidget
{
    Q_OBJECT

    ADD_PTR_PROPERTY(QStackedLayout, m_layout, getLayout)
    ADD_PTR_PROPERTY(CListWidget, m_list, getList)
    ADD_PTR_PROPERTY(CVBoxLayout, m_mainLayout, getMainLayout)
    ADD_PTR_PROPERTY(CSplitter, m_splitter, getSplitter)

    void setGraphicElements();
    // Adds a page: its entry in the list and its view in the stack, at the
    // same position (Registry: the list row is the stack index).
    void addPage(const QString &title, const QString &description, const QString &iconName,
                 std::unique_ptr<CAbstractSettingsItemController> controller);
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
    std::vector<std::unique_ptr<CAbstractSettingsItemController>> m_pages;
};

} // namespace wpw

#endif // SETTINGSWIDGET_H
