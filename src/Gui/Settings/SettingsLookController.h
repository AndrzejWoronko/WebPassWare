#ifndef SETTINGSLOOKCONTROLLER_H
#define SETTINGSLOOKCONTROLLER_H

#include "Global.h"
#include "AbstractSettingsItemController.h"
#include "SettingsLookView.h"

class CApplication;

class SettingsLookController : public CAbstractSettingsItemController
{
    Q_OBJECT

public:
    // Font, colours and style apply to \a app at once.
    explicit SettingsLookController(CApplication &app, QWidget *parent = nullptr);

    virtual void checkChanges();

public slots:

    void getValuesFromSettings(void);
    void setValuesToSettings(void);

};

#endif // SETTINGSLOOKCONTROLLER_H
