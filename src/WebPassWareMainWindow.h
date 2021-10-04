#ifndef CWEBPASSWAREMAINWINDOW_H
#define CWEBPASSWAREMAINWINDOW_H

#include "Global.h"
#include "AbstractMainWindow.h"

#include "AbstractDialogStateManager.h"

class CWebPassWareMainWindow : public CAbstractMainWindow
{

   Q_OBJECT

public:
    explicit CWebPassWareMainWindow(QWidget *parent = NULL);
    ~CWebPassWareMainWindow();

private:

    virtual void setInterface(void);
    virtual void setMenu(void);
    virtual void setActions(void);
    virtual void setToolBar(void);
    virtual void setConnections(void);

public slots:

    void refreshInfo();

};

#endif // CWEBPASSWAREMAINWINDOW_H
