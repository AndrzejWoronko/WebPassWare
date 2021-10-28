#ifndef CABSTRACTSETTINGSITEMCONTROLLER_H
#define CABSTRACTSETTINGSITEMCONTROLLER_H

#include "Global.h"
#include <QWidget>
#include "AbstractSettingsItemView.h"

class CAbstractSettingsItemController : public QWidget
{
    Q_OBJECT

private:
      ADD_FIELD(QString, m_widget_name, getWidgetName, setWidgetName)

      ADD_PTR_PRO(CAbstractSettingsItemView, m_view, getView)

public:
    explicit CAbstractSettingsItemController(QWidget *parent = NULL);
    ~CAbstractSettingsItemController();

public slots:
    virtual void checkChanges() = 0;

    void enableButtons(void);
    void disableButtons(void);

signals:
    void changesWereMade(void);


};
#endif // CABSTRACTSETTINGSITEMCONTROLLER_H
