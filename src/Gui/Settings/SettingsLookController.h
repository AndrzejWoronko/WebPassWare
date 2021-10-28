#ifndef SETTINGSLOOKCONTROLLER_H
#define SETTINGSLOOKCONTROLLER_H

#include "Global.h"
#include "AbstractSettingsItemController.h"
#include "SettingsLookView.h"

class SettingsLookController : public CAbstractSettingsItemController
{
    Q_OBJECT

public:
    SettingsLookController(QWidget *parent = NULL);

    virtual void checkChanges();

public slots:

    void getValuesFromSettings(void);
    void setValuesToSettings(void);

};

#endif // SETTINGSLOOKCONTROLLER_H
