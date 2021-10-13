#ifndef CWEBPASSWAREMAINWINDOW_H
#define CWEBPASSWAREMAINWINDOW_H

#include <QFrame>
#include <QLabel>
#include <QTreeView>
#include <QTextEdit>
#include <QLineEdit>
#include <QSqlQuery>
#include <QSqlTableModel>

#include "Global.h"
#include "AbstractMainWindow.h"

#include "AbstractDialogStateManager.h"
#include "TabWidget.h"
#include "TableView.h"
#include "ToolButton.h"
#include "Frame.h"
#include "DelayEditLine.h"


class CWebPassWareMainWindow : public CAbstractMainWindow
{

   Q_OBJECT
    ADD_PTR_PROPERTY(QLabel, m_dateLabel, getDateLabel)
    ADD_PTR_PROPERTY(QLabel, m_dbLabel, getDbLabel)

    ADD_PTR_PROPERTY(CHBoxLayout, m_HLayoutMain, getHLayoutMain)
    ADD_PTR_PROPERTY(CVBoxLayout, m_VLayoutMain, getVLayoutMain)

    ADD_PTR_PROPERTY(QTreeView, m_treeGroupList, getTreeGroupList)

    ADD_PTR_PROPERTY(CTabWidget, m_tabWidget, getTabWidget)
    ADD_PTR_PROPERTY(CSplitter, m_splitter, getSplitter)


    ADD_PTR_PROPERTY(CTableView, m_dataTable, getDataTable)

    ADD_PTR_PROPERTY(CDelayEditLine, m_filtrGroupList, getFiltrGroupList)
    ADD_PTR_PROPERTY(CDelayEditLine, m_filtrDataTable, getFiltrDataTable)

public:
    explicit CWebPassWareMainWindow(QWidget *parent = NULL);
    ~CWebPassWareMainWindow();

private:

    virtual void setInterface(void);
    virtual void setMenu(void);
    virtual void setActions(void);
    virtual void setToolBar(void);
    virtual void setConnections(void);


    QWidget *initTabData();

public slots:

    void refreshInfo();
    void on_ACTION_ABOUT_triggered();

};

#endif // CWEBPASSWAREMAINWINDOW_H
