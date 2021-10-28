#ifndef SETTINGSDATABASEVIEWCONTROLLER_H
#define SETTINGSDATABASEVIEWCONTROLLER_H


#include "Global.h"
#include "AbstractSettingsItemController.h"
#include "SettingsDatabaseView.h"

class SettingsDatabaseViewController : public CAbstractSettingsItemController
{
    Q_OBJECT

public:
    SettingsDatabaseViewController(QWidget *parent = NULL);

    virtual void checkChanges();

public slots:

    void getValuesFromSettings(void);
    void setValuesToSettings(void);

};

#endif // SETTINGSDATABASEVIEWCONTROLLER_H
