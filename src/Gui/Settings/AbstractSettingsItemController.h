#ifndef CABSTRACTSETTINGSITEMCONTROLLER_H
#define CABSTRACTSETTINGSITEMCONTROLLER_H

#include "Global.h"
#include <QWidget>
#include "AbstractSettingsItemView.h"

namespace wpw {

// Drives one settings page (m_view, owned and deleted here; the settings
// widget shows it).
class CAbstractSettingsItemController : public QObject
{
    Q_OBJECT

private:
      ADD_FIELD(QString, m_widget_name, getWidgetName, setWidgetName)

      ADD_PTR_PRO(CAbstractSettingsItemView, m_view, getView)

public:
    explicit CAbstractSettingsItemController(QWidget *parent = nullptr);
    ~CAbstractSettingsItemController();

public slots:
    virtual void checkChanges() = 0;

    void enableButtons(void);
    void disableButtons(void);

signals:
    void changesWereMade(void);


};

} // namespace wpw

#endif // CABSTRACTSETTINGSITEMCONTROLLER_H
