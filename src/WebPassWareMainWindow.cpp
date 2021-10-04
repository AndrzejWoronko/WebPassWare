#include "WebPassWareMainWindow.h"

#include "Application.h"

CWebPassWareMainWindow::CWebPassWareMainWindow(QWidget *parent) : CAbstractMainWindow(QString("WebPassWareMainWindow"), parent)
{
    setAdditionalWidgets();
}

CWebPassWareMainWindow::~CWebPassWareMainWindow()
{

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

    CAction *actionHelp = new CAction(tr("&About program"), ICON("About"), tr("About program"), QString("Ctrl+H"), QString("ACTION_HELP"), this);
    m_actions.insert(actionHelp->getActionName(), actionHelp);
}

void CWebPassWareMainWindow::setMenu(void)
{
    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *helpMenu;

    fileMenu = new QMenu(m_menuBar);
    fileMenu->setTitle(tr("&File"));
    fileMenu->addAction(m_actions.value(QString("ACTION_REFRESH_TABLE_LIST")));
    fileMenu->addSeparator();
    fileMenu->addAction(m_actions.value(QString("ACTION_EXIT")));

    toolMenu = new QMenu(m_menuBar);
    toolMenu->setTitle(tr("&Tools"));

    helpMenu = new QMenu(m_menuBar);
    helpMenu->setTitle(tr("&Help"));
    helpMenu->addAction(m_actions.value(QString("ACTION_HELP")));
    helpMenu->addAction(m_actions.value(QString("ACTION_VISIT_WEBSITE")));
    helpMenu->addAction(m_actions.value(QString("ACTION_ABOUT_QT")));

    m_menus << fileMenu << toolMenu << helpMenu;
    return;
}

void CWebPassWareMainWindow::setToolBar(void)
{
    m_toolBar->addAction(m_actions.value(QString("ACTION_REFRESH_TABLE_LIST")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_EXIT")));
    m_toolBar->addSeparator();    
    m_toolBar->addAction(m_actions.value(QString("ACTION_HELP")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_VISIT_WEBSITE")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_ABOUT_QT")));
    return;
}

void CWebPassWareMainWindow::setConnections(void)
{

    //Odświerzenie info w status bar
    QTimer::singleShot(1000, this, SLOT(refreshInfo()));

    //connect(m_filtr_table_list, SIGNAL(delayEditingFinished(const QString &)), m_table_list_proxy_model, SLOT(setFilterFixedString(const QString &)));
    //connect(m_filtr_table_data, SIGNAL(delayEditingFinished(const QString &)), m_table_data_proxy_model, SLOT(setFilterFixedString(const QString &)));
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

    CToolButton *addRowButton = new CToolButton(CButtonPrivate(tr("Add record"), tr("Add record"), ICON("Add-row")), tab);
    addRowButton->setDefaultAction(m_actions.value("ACTION_ADD_ROW"));
    CToolButton *delRowButton = new CToolButton(CButtonPrivate(tr("Delete record"), tr("Delete record"), ICON("Delete-row")), tab);
    delRowButton->setDefaultAction(m_actions.value("ACTION_DEL_ROW"));
    CToolButton *copyDataButton = new CToolButton(CButtonPrivate(tr("Copy record"), tr("Copy record"), ICON("Copy")), tab);
    copyDataButton->setDefaultAction(m_actions.value("ACTION_COPY_DATA"));

    CToolButton *refreshDataButton = new CToolButton(CButtonPrivate(tr("Refresh records"), tr("Refresh records"), ICON("Refresh")), tab);
    refreshDataButton->setDefaultAction(m_actions.value("ACTION_REFRESH_DATA"));

    hboxLayout->addWidget(addRowButton);
    hboxLayout->addWidget(delRowButton);
    hboxLayout->addWidget(copyDataButton);
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

