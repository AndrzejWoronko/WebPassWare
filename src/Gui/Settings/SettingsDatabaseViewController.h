#ifndef SETTINGSDATABASEVIEWCONTROLLER_H
#define SETTINGSDATABASEVIEWCONTROLLER_H


#include "Global.h"
#include "AbstractSettingsItemController.h"
#include "SettingsDatabaseView.h"

namespace wpw {

class SettingsDatabaseViewController : public CAbstractSettingsItemController
{
    Q_OBJECT

public:
    explicit SettingsDatabaseViewController(CDatabase &db, QWidget *parent = nullptr);

    virtual void checkChanges();

public slots:

    void getValuesFromSettings(void);
    void setValuesToSettings(void);

private:
    CDatabase &m_db;
};

} // namespace wpw

#endif // SETTINGSDATABASEVIEWCONTROLLER_H
