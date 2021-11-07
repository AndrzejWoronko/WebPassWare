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
#include "SqlModel.h"

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

    ADD_PTR_PROPERTY(QMenu, m_headerContextMenu, getHeaderContextMenu)

public:
    explicit CWebPassWareMainWindow(QWidget *parent = NULL);
    ~CWebPassWareMainWindow();

private:

    bool m_visible_passwords;
    CAction *m_visible_passwords_action;

    CSqlModel *m_pass_group_model;
    CSqlModel *m_pass_entry_model;

    QSortFilterProxyModel *m_pass_group_proxy_model;
    QSortFilterProxyModel *m_pass_entry_proxy_model;

    virtual void setInterface(void);
    virtual void setMenu(void);
    virtual void setActions(void);
    virtual void setToolBar(void);
    virtual void setConnections(void);    

    qint64 getCurrentPassGroupId();
    qint64 getCurrentPassEntryId();

    QWidget *initTabData();
    void initContextMenu();

public slots:

    void refreshInfo();
    void showTreeTableListContextMenu(const QPoint &position);
    void showDataTableContextMenu(const QPoint &position);
    void filterTableData(const QModelIndex &);
    void onCustomHeaderContextMenuRequested(const QPoint& pos);

    void on_ACTION_ABOUT_triggered();
    void on_ACTION_REFRESH_ALL_triggered();

    void on_ACTION_REFRESH_PASS_GROUP_triggered();
    void on_ACTION_ADD_PASS_GROUP_triggered();
    void on_ACTION_EDIT_PASS_GROUP_triggered();
    void on_ACTION_DEL_PASS_GROUP_triggered();


    void on_ACTION_REFRESH_PASS_ENTRY_triggered();
    void on_ACTION_ADD_PASS_ENTRY_triggered();
    void on_ACTION_EDIT_PASS_ENTRY_triggered();
    void on_ACTION_DEL_PASS_ENTRY_triggered();
    void on_ACTION_COPY_PASS_ENTRY_PASSWORD_triggered();
    void on_ACTION_COPY_PASS_ENTRY_USER_triggered();

    void on_ACTION_GENERATOR_DIALOG_triggered();

    void on_ACTION_SETTINGS_DIALOG_triggered();

    void on_ACTION_DATA_EXPORT_DIALOG_triggered();
};

#endif // CWEBPASSWAREMAINWINDOW_H
