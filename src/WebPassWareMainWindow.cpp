#include "WebPassWareMainWindow.h"
#include "AboutDialog.h"
#include "Application.h"
#include "PassEntryService.h"
#include "PassGroupService.h"
#include "PassEntryDialogController.h"
#include "PassGroupDialogController.h"

CWebPassWareMainWindow::CWebPassWareMainWindow(QWidget *parent) : CAbstractMainWindow(QString("WebPassWareMainWindow"), parent)
{
    setAdditionalWidgets();
}

CWebPassWareMainWindow::~CWebPassWareMainWindow()
{
    safe_delete(m_pass_group_proxy_model)
    safe_delete(m_pass_entry_proxy_model)
    safe_delete(m_pass_group_model)
    safe_delete(m_pass_entry_model)
}

void CWebPassWareMainWindow::setInterface(void)
{
    this->setWindowIconText(CApplication::toString());
    this->setDocumentMode(true);
    this->setWindowTitle(tr(APP_NAME));
    this->setIconName(ICON(CApplication::iconName()));

    this->resize(850, 700);
    this->setMinimumSize(QSize(800, 600));

    m_HLayoutMain = new CHBoxLayout(m_centralwidget);

    //Splitter
    m_splitter = new CSplitter(QString("WebPassWareMainWindowSplitter"), Qt::Horizontal, m_centralwidget);

    QWidget *table_widget  = new QWidget(m_splitter);
    m_VLayoutMain = new CVBoxLayout(table_widget);
    m_filtrGroupList = new CDelayEditLine(this);
    m_filtrGroupList->setPlaceholderText(tr("Group filter ... "));
    m_VLayoutMain->addWidget(m_filtrGroupList);
    //Table list
    m_treeGroupList = new QTreeView(table_widget);
    m_VLayoutMain->addWidget(m_treeGroupList);


    //Tab widget
    m_tabWidget = new CTabWidget(m_splitter);
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(20);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(m_tabWidget->sizePolicy().hasHeightForWidth());
    m_tabWidget->setSizePolicy(sizePolicy1);

    m_tabWidget->addTabWidget(initTabData(), tr("Records"), ICON("Password"));
    m_tabWidget->setTabEnabled(0, true);
    m_tabWidget->hideTabButton(0);


    m_splitter->addWidget(table_widget);
    m_splitter->addWidget(m_tabWidget);

    m_HLayoutMain->addWidget(m_splitter);
    m_splitter->restoreSplitterState();

    m_dbLabel = new QLabel(this);
    m_statusBar->addPermanentWidget(m_dbLabel);
    m_statusBar->addPermanentWidget(new CVLine());
    m_dateLabel = new QLabel(this);
    m_statusBar->addPermanentWidget(m_dateLabel);
}

void CWebPassWareMainWindow::setActions(void)
{

    CAction *actionAbount = new CAction(tr("&About program"), ICON("About"), tr("About program"), QString("Ctrl+H"), QString("ACTION_ABOUT"), this);
    m_actions.insert(actionAbount->getActionName(), actionAbount);

    CAction *action_RefreshPassGroup = new CAction(tr("Refresh group"), ICON("Refresh"), tr("Refresh group"), QString(""), QString("ACTION_REFRESH_PASS_GROUP"), this);
    m_actions.insert(action_RefreshPassGroup->getActionName(), action_RefreshPassGroup);

    CAction *action_AddPassGroup = new CAction(tr("Add group"), ICON("Add-row"), tr("Add group"), QString("Insert"), QString("ACTION_ADD_PASS_GROUP"), this);
    m_actions.insert(action_AddPassGroup->getActionName(), action_AddPassGroup);
    CAction *action_EditPassGroup = new CAction(tr("Edit group"), ICON("Edit-row"), tr("Edit group"), QString("Enter"), QString("ACTION_EDIT_PASS_GROUP"), this);
    m_actions.insert(action_EditPassGroup->getActionName(), action_EditPassGroup);
    CAction *action_DelPassGroup = new CAction(tr("Delete group"), ICON("Delete-row"), tr("Delete group"), QString("Delete"), QString("ACTION_DEL_PASS_GROUP"), this);
    m_actions.insert(action_DelPassGroup->getActionName(), action_DelPassGroup);


    CAction *action_RefreshPassEntry = new CAction(tr("Refresh records"), ICON("Refresh"), tr("Refresh records"), QString(""), QString("ACTION_REFRESH_PASS_ENTRY"), this);
    m_actions.insert(action_RefreshPassEntry->getActionName(), action_RefreshPassEntry);

    CAction *action_AddPassEntry = new CAction(tr("Add record"), ICON("Add-row"), tr("Add record"), QString("Insert"), QString("ACTION_ADD_PASS_ENTRY"), this);
    m_actions.insert(action_AddPassEntry->getActionName(), action_AddPassEntry);
    CAction *action_EditPassEntry = new CAction(tr("Edit record"), ICON("Edit-row"), tr("Edit record"), QString("Enter"), QString("ACTION_EDIT_PASS_ENTRY"), this);
    m_actions.insert(action_EditPassEntry->getActionName(), action_EditPassEntry);
    CAction *action_DelPassEntry = new CAction(tr("Delete record"), ICON("Delete-row"), tr("Delete record"), QString("Delete"), QString("ACTION_DEL_PASS_ENTRY"), this);
    m_actions.insert(action_DelPassEntry->getActionName(), action_DelPassEntry);

    CAction *action_CopyPassEntry = new CAction(tr("Copy record to clipboard"), ICON("Copy"), tr("Copy record to clipboard"), QString(""), QString("ACTION_COPY_PASS_ENTRY"), this);
    m_actions.insert(action_CopyPassEntry->getActionName(), action_CopyPassEntry);
}

void CWebPassWareMainWindow::setMenu(void)
{
    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *helpMenu;

    fileMenu = new QMenu(m_menuBar);
    fileMenu->setTitle(tr("&File"));
    fileMenu->addAction(m_actions.value(QString("ACTION_REFRESH_PASS_GROUP")));
    fileMenu->addAction(m_actions.value(QString("ACTION_REFRESH_PASS_ENTRY")));
    fileMenu->addSeparator();
    fileMenu->addAction(m_actions.value(QString("ACTION_EXIT")));

    toolMenu = new QMenu(m_menuBar);
    toolMenu->setTitle(tr("&Tools"));

    helpMenu = new QMenu(m_menuBar);
    helpMenu->setTitle(tr("&Help"));
    helpMenu->addAction(m_actions.value(QString("ACTION_ABOUT")));
    helpMenu->addAction(m_actions.value(QString("ACTION_VISIT_WEBSITE")));
    helpMenu->addAction(m_actions.value(QString("ACTION_ABOUT_QT")));

    m_menus << fileMenu << toolMenu << helpMenu;
    return;
}

void CWebPassWareMainWindow::setToolBar(void)
{
    m_toolBar->addAction(m_actions.value(QString("ACTION_REFRESH_PASS_GROUP")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_REFRESH_PASS_ENTRY")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_EXIT")));
    m_toolBar->addSeparator();    
    m_toolBar->addAction(m_actions.value(QString("ACTION_ABOUT")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_VISIT_WEBSITE")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_ABOUT_QT")));
    return;
}

void CWebPassWareMainWindow::setConnections(void)
{
//Modele proxy
    m_pass_group_proxy_model = new QSortFilterProxyModel(this);
    m_pass_group_proxy_model->setDynamicSortFilter(true);
    m_pass_group_proxy_model->setFilterKeyColumn(-1);
    m_pass_group_proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_pass_entry_proxy_model = new QSortFilterProxyModel(this);
    m_pass_entry_proxy_model->setDynamicSortFilter(true);
    m_pass_entry_proxy_model->setFilterKeyColumn(-1);
    m_pass_entry_proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);

//Modele danych
    m_pass_group_model = PassGroupService::getInstance().getSqlModel();
    m_pass_entry_model = PassEntryService::getInstance().getSqlModel();

    m_pass_group_proxy_model->setSourceModel(m_pass_group_model);
    m_pass_entry_proxy_model->setSourceModel(m_pass_entry_model);

//Podłącznie widoków do tabel
    m_treeGroupList->setModel(m_pass_group_proxy_model);
    m_dataTable->setModel(m_pass_entry_proxy_model);

//Załadowanie danych do modelu
    m_pass_group_model->refresh();
    m_pass_entry_model->refresh();

//Tabela lista tabel połaczenia
    m_treeGroupList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeGroupList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showTreeTableListContextMenu(const QPoint &)));
    //connect(m_treeTableList, SIGNAL(activated(const QModelIndex &)), this, SLOT(showTableData(const QModelIndex &)));
    //connect(m_treeGroupList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(showTableData(const QModelIndex &)));

//Tabela danych połaczenia
    m_dataTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_dataTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showDataTableContextMenu(const QPoint &)));

//Odświerzenie info w status bar
    QTimer::singleShot(1000, this, SLOT(refreshInfo()));

    connect(m_filtrGroupList, SIGNAL(delayEditingFinished(const QString &)), m_pass_group_proxy_model, SLOT(setFilterFixedString(const QString &)));
    connect(m_filtrDataTable, SIGNAL(delayEditingFinished(const QString &)), m_pass_entry_proxy_model, SLOT(setFilterFixedString(const QString &)));
}

QWidget *CWebPassWareMainWindow::initTabData()
{
    QWidget *tab = new QWidget();
    auto vboxLayout = new CVBoxLayout(tab);

    m_filtrDataTable = new CDelayEditLine(tab);
    m_filtrDataTable->setPlaceholderText(tr("Filter ..."));
    vboxLayout->addWidget(m_filtrDataTable);

    m_dataTable = new CTableView(tab);
    vboxLayout->addWidget(m_dataTable);

    auto hboxLayout = new CHBoxLayout();

    CToolButton *copyDataButton = new CToolButton(CButtonPrivate(tr("To clipboard"), tr("Copy record to clipboard"), ICON("Copy")), tab);
    copyDataButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    copyDataButton->setDefaultAction(m_actions.value("ACTION_COPY_PASS_ENTRY"));

    CToolButton *addRowButton = new CToolButton(CButtonPrivate(tr("Add record"), tr("Add record"), ICON("Add-row")), tab);
    addRowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addRowButton->setDefaultAction(m_actions.value("ACTION_ADD_PASS_ENTRY"));
    CToolButton *editRowButton = new CToolButton(CButtonPrivate(tr("Edit record"), tr("Edit record"), ICON("Edit-row")), tab);
    editRowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    editRowButton->setDefaultAction(m_actions.value("ACTION_EDIT_PASS_ENTRY"));    
    CToolButton *delRowButton = new CToolButton(CButtonPrivate(tr("Delete record"), tr("Delete record"), ICON("Delete-row")), tab);
    delRowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    delRowButton->setDefaultAction(m_actions.value("ACTION_DEL_PASS_ENTRY"));
    CToolButton *refreshDataButton = new CToolButton(CButtonPrivate(tr("Refresh records"), tr("Refresh records"), ICON("Refresh")), tab);
    refreshDataButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    refreshDataButton->setDefaultAction(m_actions.value("ACTION_REFRESH_PASS_ENTRY"));

    hboxLayout->addWidget(copyDataButton);
    hboxLayout->addWidget(addRowButton);
    hboxLayout->addWidget(editRowButton);
    hboxLayout->addWidget(delRowButton);    
    QSpacerItem *spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem);
    hboxLayout->addWidget(refreshDataButton);
    vboxLayout->addLayout(hboxLayout);

    return tab;
}

void CWebPassWareMainWindow::refreshInfo()
{
//    m_dbLabel->setText(tr("Baza: %1, host: %2, baza: %3").arg(DB.getMBaseType()).arg(DB.getMBaseHost()).arg(DB.getMBaseName()));
    m_dateLabel->setText(tr("Date: <b>%1</b> %2").arg(QDate::currentDate().toString(DATE_FORMAT)).arg(QTime::currentTime().toString(TIME_FORMAT)));
    QTimer::singleShot(1000, this, SLOT(refreshInfo()));
}

void CWebPassWareMainWindow::on_ACTION_ABOUT_triggered()
{
    CAboutDialog dialog(this);
    dialog.addFieldsAndComponents();
    dialog.exec();
}


void CWebPassWareMainWindow::showTreeTableListContextMenu(const QPoint &position)
{
    QMenu contextmenu(this);

    contextmenu.addAction(m_actions.value(QString("ACTION_ADD_PASS_GROUP")));
    contextmenu.addAction(m_actions.value(QString("ACTION_EDIT_PASS_GROUP")));
    contextmenu.addAction(m_actions.value(QString("ACTION_DEL_PASS_GROUP")));
    contextmenu.addSeparator();
    contextmenu.addAction(m_actions.value(QString("ACTION_REFRESH_PASS_GROUP")));

    contextmenu.exec(m_treeGroupList->mapToGlobal(position));
}

void CWebPassWareMainWindow::showDataTableContextMenu(const QPoint &position)
{
    QMenu contextmenu(this);

    contextmenu.addAction(m_actions.value(QString("ACTION_ADD_PASS_ENTRY")));
    contextmenu.addAction(m_actions.value(QString("ACTION_EDIT_PASS_ENTRY")));
    contextmenu.addAction(m_actions.value(QString("ACTION_DEL_PASS_ENTRY")));
    contextmenu.addAction(m_actions.value(QString("ACTION_COPY_PASS_ENTRY")));
    contextmenu.addSeparator();
    contextmenu.addAction(m_actions.value(QString("ACTION_REFRESH_PASS_ENTRY")));
    contextmenu.exec(m_dataTable->mapToGlobal(position));
}

void CWebPassWareMainWindow::on_ACTION_REFRESH_PASS_GROUP_triggered()
{
    if (m_pass_group_model)
        m_pass_group_model->refresh();
}

void CWebPassWareMainWindow::on_ACTION_ADD_PASS_GROUP_triggered()
{
    DEBUG_WITH_LINE << "ADD PASS GROUP";
    auto dialog_ctrl = new PassGroupDialogController(m_treeGroupList);
    if (dialog_ctrl->exec("Dodanie grupy"))
    {
        if (m_pass_group_model)
            m_pass_group_model->refresh();
    }
    safe_delete(dialog_ctrl)
}

void CWebPassWareMainWindow::on_ACTION_EDIT_PASS_GROUP_triggered()
{
    DEBUG_WITH_LINE << "EDIT PASS GROUP";
    auto dialog_ctrl = new PassGroupDialogController(m_treeGroupList);
    if (dialog_ctrl->exec(0, tr("Edycja grupy")))
    {
        if (m_pass_group_model)
            m_pass_group_model->refresh();
    }
    safe_delete(dialog_ctrl)
}

void CWebPassWareMainWindow::on_ACTION_DEL_PASS_GROUP_triggered()
{
    DEBUG_WITH_LINE << "DEL PASS GROUP";
}

void CWebPassWareMainWindow::on_ACTION_REFRESH_PASS_ENTRY_triggered()
{
    if (m_pass_entry_model)
        m_pass_entry_model->refresh();
}

void CWebPassWareMainWindow::on_ACTION_ADD_PASS_ENTRY_triggered()
{
    DEBUG_WITH_LINE << "ADD PASS ENTRY";
    auto dialog_ctrl = new PassEntryDialogController(m_tabWidget);
    if (dialog_ctrl->exec(tr("Dodanie rekordu")))
       {
            if (m_pass_entry_model)
                m_pass_entry_model->refresh();
       }
    safe_delete(dialog_ctrl)
}

void CWebPassWareMainWindow::on_ACTION_EDIT_PASS_ENTRY_triggered()
{
    DEBUG_WITH_LINE << "EDIT PASS ENTRY";
    auto dialog_ctrl = new PassEntryDialogController(m_tabWidget);
    if (dialog_ctrl->exec(0, tr("Edycja rekordu")))
       {
            if (m_pass_entry_model)
                m_pass_entry_model->refresh();
       }
    safe_delete(dialog_ctrl)
}

void CWebPassWareMainWindow::on_ACTION_DEL_PASS_ENTRY_triggered()
{
    DEBUG_WITH_LINE << "DEL PASS ENTRY";
}

void CWebPassWareMainWindow::on_ACTION_COPY_PASS_ENTRY_triggered()
{
    DEBUG_WITH_LINE << "COPY PASS ENTRY";
}
