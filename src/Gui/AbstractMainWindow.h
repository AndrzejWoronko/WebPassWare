#ifndef CABSTRACTMAINWINDOW_H
#define CABSTRACTMAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>
#include <QToolBar>
#include <QClipboard>
#include <QDesktopWidget>
#include <QScreen>

#include "Global.h"
#include "Layouts.h"
#include "AbstractDialogStateManager.h"
#include "Action.h"
#include "Splitter.h"


class CAbstractMainWindow : public QMainWindow, public CAbstractDialogStateManager,  public Singleton<CAbstractMainWindow>
{
    Q_OBJECT

    ADD_FIELD(QString, m_main_window_name, getMainWindowName, setMainWindowName)

    ADD_PTR_PRO(QMenuBar, m_menuBar, getMenuBar)
    ADD_PTR_PRO(QStatusBar, m_statusBar, getStatusBar)
    ADD_PTR_PRO(QToolBar, m_toolBar, getToolBar)
    ADD_PTR_PRO(QWidget, m_centralwidget, getCentralWidget)

public:

    CAbstractMainWindow(const QString &name, QWidget *parent = NULL);
    ~CAbstractMainWindow();

    void centerAndResize();
    void setIconName(const QString &iconName = QString());
    CAction *getAction(const QString &actionName);
    QHash <QString, CAction*> getActionsList();

    void setAdditionalWidgets();

    virtual void setInterface() = 0;
    virtual void setMenu() = 0;
    virtual void setActions() = 0;
    virtual void setToolBar(void) = 0;
    virtual void setConnections() = 0;

protected:

    QList<QMenu*> m_menus; //Lista menu
    QHash <QString, CAction*> m_actions;  //Lista akcji

private:

    void initMainWidgets();
    void initInterface();
    void initActions();
    //void initConnections();

private slots:

    virtual void restoreDialogState();
    virtual void saveDialogState();
    virtual void closeEvent(QCloseEvent *event);

public slots:

    void on_ACTION_ABOUT_QT_triggered();
    void on_ACTION_EXIT_triggered();
};

#endif // CABSTRACTMAINWINDOW_H
