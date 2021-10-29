#include "WebPassWareMainWindow.h"
#include "AboutDialog.h"
#include "Application.h"
#include "PassEntryService.h"
#include "PassGroupService.h"
#include "PassEntryDialogController.h"
#include "PassGroupDialogController.h"
#include "PasswordGenerateDialog.h"
#include "MessageBox.h"
#include "SettingsWidget.h"
#include "SettingsDialog.h"

CWebPassWareMainWindow::CWebPassWareMainWindow(QWidget *parent) : CAbstractMainWindow(QString("WebPassWareMainWindow"), parent)
{
    SETT.setDefaultGuiSettings();
//Ustawienie fontu dla całej aplikacji
    QFont f;
    if (f.fromString(SETT.getValue(SETTINGS_GUI_FONT).toString()))
        APPI->setFont(f);

    setAdditionalWidgets();
    m_dataTable->setFocus();
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

    m_tabWidget->addTabWidget(initTabData(), tr("Katalog haseł"), ICON("Password"));
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
    CAction *actionAbount = new CAction(tr("O programie ..."), ICON("About"), tr("Informacje o programie"), QString("Ctrl+H"), QString("ACTION_ABOUT"), this);
    m_actions.insert(actionAbount->getActionName(), actionAbount);

    CAction *action_RefreshPassGroup = new CAction(tr("Odświeżenie grup"), ICON("Refresh"), tr("Odświeżenie grup haseł"), QString(""), QString("ACTION_REFRESH_PASS_GROUP"), this);
    m_actions.insert(action_RefreshPassGroup->getActionName(), action_RefreshPassGroup);

    CAction *action_AddPassGroup = new CAction(tr("Dodanie grupy"), ICON("Add-row"), tr("Dodanie grupy"), QString(""), QString("ACTION_ADD_PASS_GROUP"), this);
    m_actions.insert(action_AddPassGroup->getActionName(), action_AddPassGroup);
    CAction *action_EditPassGroup = new CAction(tr("Edycja grupy"), ICON("Edit-row"), tr("Edycja grupy"), QString(""), QString("ACTION_EDIT_PASS_GROUP"), this);
    m_actions.insert(action_EditPassGroup->getActionName(), action_EditPassGroup);
    CAction *action_DelPassGroup = new CAction(tr("Usunięcie grupy"), ICON("Delete-row"), tr("Usunięcie grupy"), QString(""), QString("ACTION_DEL_PASS_GROUP"), this);
    m_actions.insert(action_DelPassGroup->getActionName(), action_DelPassGroup);


    CAction *action_RefreshPassEntry = new CAction(tr("Odświeżenie rekordów"), ICON("Refresh"), tr("Odświeżenie rekordów"), QString(""), QString("ACTION_REFRESH_PASS_ENTRY"), this);
    m_actions.insert(action_RefreshPassEntry->getActionName(), action_RefreshPassEntry);

    CAction *action_AddPassEntry = new CAction(tr("Dodawanie wpisu"), ICON("Key|Add"), tr("Dodawanie wpisu"), QString("Insert"), QString("ACTION_ADD_PASS_ENTRY"), this);
    m_actions.insert(action_AddPassEntry->getActionName(), action_AddPassEntry);
    CAction *action_EditPassEntry = new CAction(tr("Edycja wpisu"), ICON("Key|Edit"), tr("Edycja wpisu"), QString("Enter"), QString("ACTION_EDIT_PASS_ENTRY"), this);
    m_actions.insert(action_EditPassEntry->getActionName(), action_EditPassEntry);
    CAction *action_DelPassEntry = new CAction(tr("Kasowanie wpisu"), ICON("Key|Delete"), tr("Kasowanie wpisu"), QString("Delete"), QString("ACTION_DEL_PASS_ENTRY"), this);
    m_actions.insert(action_DelPassEntry->getActionName(), action_DelPassEntry);

    CAction *action_CopyPassEntryUser = new CAction(tr("Kopiowanie użytkownika do schowka"), ICON("User"), tr("Kopiowanie użytkownika do schowka"), QString("Ctrl+X"), QString("ACTION_COPY_PASS_ENTRY_USER"), this);
    m_actions.insert(action_CopyPassEntryUser->getActionName(), action_CopyPassEntryUser);

    CAction *action_CopyPassEntryPassword = new CAction(tr("Kopiowanie hasła do schowka"), ICON("Copy"), tr("Kopiowanie hasła do schowka"), QString("Ctrl+C"), QString("ACTION_COPY_PASS_ENTRY_PASSWORD"), this);
    m_actions.insert(action_CopyPassEntryPassword->getActionName(), action_CopyPassEntryPassword);

    CAction *action_RefreshAll = new CAction(tr("Odświeżenie danych"), ICON("Refresh"), tr("Odświeżenie danych"), QString(""), QString("ACTION_REFRESH_ALL"), this);
    m_actions.insert(action_RefreshAll->getActionName(), action_RefreshAll);

    CAction *action_GenerationDialog = new CAction(tr("Generator haseł"), ICON("Toolbox"), tr("Generator haseł"), QString(""), QString("ACTION_GENERATOR_DIALOG"), this);
    m_actions.insert(action_GenerationDialog->getActionName(), action_GenerationDialog);

    CAction *actionSettingsDialog = new CAction(tr("Opcje programu"), ICON("Settings"), tr("Opcje programu"), QString(""), QString("ACTION_SETTINGS_DIALOG") ,this);
    m_actions.insert(actionSettingsDialog->getActionName(), actionSettingsDialog);
}

void CWebPassWareMainWindow::setMenu(void)
{
    QMenu *fileMenu;
    QMenu *groupMenu;
    QMenu *entryMenu;
    QMenu *settingsMenu;
    QMenu *toolMenu;
    QMenu *helpMenu;

    fileMenu = new QMenu(m_menuBar);
    fileMenu->setTitle(tr("&Plik"));
    fileMenu->addAction(m_actions.value(QString("ACTION_REFRESH_ALL")));
    fileMenu->addSeparator();
    fileMenu->addAction(m_actions.value(QString("ACTION_EXIT")));

    groupMenu = new QMenu(m_menuBar);
    groupMenu->setTitle(tr("&Grupy"));
    groupMenu->addAction(m_actions.value(QString("ACTION_ADD_PASS_GROUP")));
    groupMenu->addSeparator();
    groupMenu->addAction(m_actions.value(QString("ACTION_REFRESH_PASS_GROUP")));

    entryMenu = new QMenu(m_menuBar);
    entryMenu->setTitle(tr("&Wpisy"));
    entryMenu->addAction(m_actions.value(QString("ACTION_ADD_PASS_ENTRY")));
    entryMenu->addSeparator();
    entryMenu->addAction(m_actions.value(QString("ACTION_REFRESH_PASS_ENTRY")));

    settingsMenu = new QMenu(m_menuBar);
    settingsMenu->setTitle(tr("&Ustawienia"));
    settingsMenu->addAction(m_actions.value(QString("ACTION_SETTINGS_DIALOG")));

    toolMenu = new QMenu(m_menuBar);
    toolMenu->setTitle(tr("&Narzędzia"));
    toolMenu->addAction(m_actions.value(QString("ACTION_GENERATOR_DIALOG")));

    helpMenu = new QMenu(m_menuBar);
    helpMenu->setTitle(tr("&Pomoc"));
    helpMenu->addAction(m_actions.value(QString("ACTION_ABOUT")));
    helpMenu->addAction(m_actions.value(QString("ACTION_VISIT_WEBSITE")));
    helpMenu->addAction(m_actions.value(QString("ACTION_ABOUT_QT")));

    m_menus << fileMenu << groupMenu << entryMenu << settingsMenu << toolMenu << helpMenu;
    return;
}

void CWebPassWareMainWindow::setToolBar(void)
{    
    m_toolBar->addAction(m_actions.value(QString("ACTION_REFRESH_ALL")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_EXIT")));
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_actions.value(QString("ACTION_ADD_PASS_GROUP")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_ADD_PASS_ENTRY")));
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
    m_dataTable->resizeColumnsToContents();

//Tabela lista tabel połaczenia
    m_treeGroupList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeGroupList, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showTreeTableListContextMenu(const QPoint &)));
    //connect(m_treeTableList, SIGNAL(activated(const QModelIndex &)), this, SLOT(showTableData(const QModelIndex &)));
    connect(m_treeGroupList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(filterTableData(const QModelIndex &)));
    connect(m_treeGroupList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_ACTION_EDIT_PASS_GROUP_triggered()));

//Tabela danych połaczenia
    m_dataTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_dataTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showDataTableContextMenu(const QPoint &)));
    connect(m_dataTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_ACTION_EDIT_PASS_ENTRY_triggered()));
    connect(m_dataTable, SIGNAL(keyInsert()), this, SLOT(on_ACTION_ADD_PASS_ENTRY_triggered()));
    connect(m_dataTable, SIGNAL(keyEnter()), this, SLOT(on_ACTION_EDIT_PASS_ENTRY_triggered()));
    connect(m_dataTable, SIGNAL(keyDelete()), this, SLOT(on_ACTION_DEL_PASS_ENTRY_triggered()));

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
    m_filtrDataTable->setPlaceholderText(tr("Filtr ..."));
    vboxLayout->addWidget(m_filtrDataTable);

    m_dataTable = new CTableView(tab);
    vboxLayout->addWidget(m_dataTable);

    auto hboxLayout = new CHBoxLayout();

    CToolButton *copyDataButtonUser = new CToolButton(CButtonPrivate(tr("Użytkownik do schowka"), tr("Kopiowanie użytkownika do schowka"), ICON("User")), tab);
    copyDataButtonUser->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    copyDataButtonUser->setDefaultAction(m_actions.value("ACTION_COPY_PASS_ENTRY_USER"));
    CToolButton *copyDataButtonPass = new CToolButton(CButtonPrivate(tr("Hasło do schowka"), tr("Kopiowanie hasła do schowka"), ICON("Copy")), tab);
    copyDataButtonPass->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    copyDataButtonPass->setDefaultAction(m_actions.value("ACTION_COPY_PASS_ENTRY_PASSWORD"));
    CToolButton *addRowButton = new CToolButton(CButtonPrivate(tr("Dodanie wpisu"), tr("Dodanie wpisu"), ICON("Add-row")), tab);
    addRowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addRowButton->setDefaultAction(m_actions.value("ACTION_ADD_PASS_ENTRY"));
    CToolButton *editRowButton = new CToolButton(CButtonPrivate(tr("Edycja wpisu"), tr("Edycja wpisu"), ICON("Edit-row")), tab);
    editRowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    editRowButton->setDefaultAction(m_actions.value("ACTION_EDIT_PASS_ENTRY"));    
    CToolButton *delRowButton = new CToolButton(CButtonPrivate(tr("Kasowanie wpisu"), tr("Kasowanie wpisu"), ICON("Delete-row")), tab);
    delRowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    delRowButton->setDefaultAction(m_actions.value("ACTION_DEL_PASS_ENTRY"));
    CToolButton *refreshDataButton = new CToolButton(CButtonPrivate(tr("Odświeżenie rekordów"), tr("Odświeżenie rekordów"), ICON("Refresh")), tab);
    refreshDataButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    refreshDataButton->setDefaultAction(m_actions.value("ACTION_REFRESH_PASS_ENTRY"));

    hboxLayout->addWidget(copyDataButtonUser);
    hboxLayout->addWidget(copyDataButtonPass);
    hboxLayout->addWidget(addRowButton);
    hboxLayout->addWidget(editRowButton);
    hboxLayout->addWidget(delRowButton);    
    QSpacerItem *spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem);
    hboxLayout->addWidget(refreshDataButton);
    vboxLayout->addLayout(hboxLayout);

    return tab;
}

qint64 CWebPassWareMainWindow::getCurrentPassGroupId()
{
    QModelIndex index = m_treeGroupList->currentIndex();
    QModelIndex sourceIndex = m_pass_group_proxy_model->mapToSource(index);

    if (sourceIndex.isValid())
    {
        QModelIndex idx;
        if (m_pass_group_model)
        {
            int column = m_pass_group_model->columnIndex("id_pass_group");
            idx = m_pass_group_proxy_model->index(sourceIndex.row(), column);
        }
        qint64 id = m_pass_group_proxy_model->data(idx).toLongLong();
        return id;
    }
    else
        return -1;
}

qint64 CWebPassWareMainWindow::getCurrentPassEntryId()
{
    QModelIndex index = m_dataTable->currentIndex();
    QModelIndex sourceIndex = m_pass_entry_proxy_model->mapToSource(index);

    if (sourceIndex.isValid())
    {
        QModelIndex idx;
        if (m_pass_group_model)
        {
            int column = m_pass_entry_model->columnIndex("id_pass_entry");
            idx = m_pass_entry_proxy_model->index(sourceIndex.row(), column);
        }
        qint64 id = m_pass_entry_proxy_model->data(idx).toLongLong();
        return id;
    }
    else
        return -1;
}

void CWebPassWareMainWindow::refreshInfo()
{
//  m_dbLabel->setText(tr("Baza: %1, host: %2, baza: %3").arg(DB.getMBaseType()).arg(DB.getMBaseHost()).arg(DB.getMBaseName()));
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
    contextmenu.addAction(m_actions.value(QString("ACTION_COPY_PASS_ENTRY_PASSWORD")));
    contextmenu.addAction(m_actions.value(QString("ACTION_COPY_PASS_ENTRY_USER")));
    contextmenu.addSeparator();
    contextmenu.addAction(m_actions.value(QString("ACTION_REFRESH_PASS_ENTRY")));
    contextmenu.exec(m_dataTable->mapToGlobal(position));
}

void CWebPassWareMainWindow::filterTableData(const QModelIndex & index)
{
    QModelIndex sourceIndex = m_pass_group_proxy_model->mapToSource(index);

    if (sourceIndex.isValid())
    {
        QModelIndex idx;
        if (m_pass_group_model)
        {
            int column = m_pass_group_model->columnIndex("id_pass_group");
            idx = m_pass_group_proxy_model->index(sourceIndex.row(), column);
        }
        qint64 id = m_pass_group_proxy_model->data(idx).toLongLong();
        m_pass_entry_model->setWhere(QString("m_id_pass_group=%1").arg(id));
        DEBUG_WITH_LINE << "Clicked pass group id: " << id;
        m_pass_entry_model->refresh();
    }
}

void CWebPassWareMainWindow::on_ACTION_REFRESH_ALL_triggered()
{
    on_ACTION_REFRESH_PASS_GROUP_triggered();
    on_ACTION_REFRESH_PASS_ENTRY_triggered();
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
    qint64 id = getCurrentPassGroupId();

    if ((id > 0) && dialog_ctrl->exec(id, tr("Edycja grupy")))
    {
        if (m_pass_group_model)
            m_pass_group_model->refresh();
    }
    safe_delete(dialog_ctrl)
}

void CWebPassWareMainWindow::on_ACTION_DEL_PASS_GROUP_triggered()
{
    qint64 id =  getCurrentPassGroupId();
    if (id > 0)
    {
        QString question = tr("Czy chcesz skasować grupę o id: %1 ?").arg(id);
        if (CMessageBox::YesNoDialog(question, m_treeGroupList) == CMessageBox::Yes)
        {
            PassGroupService::getInstance().deleteObject(id);
            if (m_pass_group_model)
            {
                m_pass_entry_model->refresh();
            }
        }
    }
    DEBUG_WITH_LINE << "DEL PASS GROUP";
}

void CWebPassWareMainWindow::on_ACTION_REFRESH_PASS_ENTRY_triggered()
{
    if (m_pass_entry_model)
       {
          m_pass_entry_model->setWhere(QString());
          m_pass_entry_model->refresh();
          m_dataTable->resizeColumnsToContents();
       }
}

void CWebPassWareMainWindow::on_ACTION_ADD_PASS_ENTRY_triggered()
{
    DEBUG_WITH_LINE << "ADD PASS ENTRY";
    auto dialog_ctrl = new PassEntryDialogController(m_tabWidget);
    if (dialog_ctrl->exec(tr("Dodanie rekordu")))
       {
            if (m_pass_entry_model)
                {
                   m_pass_entry_model->refresh();
                   m_dataTable->resizeColumnsToContents();
                }
       }
    safe_delete(dialog_ctrl)
}

void CWebPassWareMainWindow::on_ACTION_EDIT_PASS_ENTRY_triggered()
{
    DEBUG_WITH_LINE << "EDIT PASS ENTRY";
    auto dialog_ctrl = new PassEntryDialogController(m_tabWidget);
    qint64 id = getCurrentPassEntryId();

    if ((id > 0) && dialog_ctrl->exec(id, tr("Edycja rekordu")))
       {
            if (m_pass_entry_model)
               {
                   m_pass_entry_model->refresh();
                   m_dataTable->resizeColumnsToContents();
               }
       }
    safe_delete(dialog_ctrl)
}

void CWebPassWareMainWindow::on_ACTION_DEL_PASS_ENTRY_triggered()
{
    qint64 id = getCurrentPassEntryId();
    if (id > 0)
       {
            auto pe = PassEntryService::getInstance().getObject(id);
            {
                QString question = tr("Czy chcesz skasować wpis o id: %1 %2?").arg(id).arg(pe->getm_title());
                if (CMessageBox::YesNoDialog(question, m_dataTable) == CMessageBox::Yes)
                {
                    PassEntryService::getInstance().deleteObject(id);
                    if (m_pass_entry_model)
                        {
                            m_pass_entry_model->refresh();
                            m_dataTable->resizeColumnsToContents();
                        }
                }
            }
            safe_delete(pe)
       }
    DEBUG_WITH_LINE << "DEL PASS ENTRY";
}

void CWebPassWareMainWindow::on_ACTION_COPY_PASS_ENTRY_PASSWORD_triggered()
{
    qint64 id = getCurrentPassEntryId();
    if (id > 0)
    {
        auto pe = PassEntryService::getInstance().getObject(id);
        if (pe)
        {
            QString text = pe->getm_pass();
            QMimeData *mimeData = new QMimeData();
            mimeData->setText(text);
            mimeData->setData("text/csv", text.toUtf8());
            QApplication::clipboard()->setMimeData(mimeData);
            DEBUG_WITH_LINE << text;
        }
        safe_delete(pe)
    }
    DEBUG_WITH_LINE << "COPY PASS ENTRY PASSWORD";
}

void CWebPassWareMainWindow::on_ACTION_COPY_PASS_ENTRY_USER_triggered()
{
    qint64 id = getCurrentPassEntryId();
    if (id > 0)
    {
        auto pe = PassEntryService::getInstance().getObject(id);
        if (pe)
        {
            QString text = pe->getm_user();
            QMimeData *mimeData = new QMimeData();
            mimeData->setText(text);
            mimeData->setData("text/csv", text.toUtf8());
            QApplication::clipboard()->setMimeData(mimeData);
            DEBUG_WITH_LINE << text;
        }
        safe_delete(pe)
    }
    DEBUG_WITH_LINE << "COPY PASS ENTRY USER";
}

void CWebPassWareMainWindow::on_ACTION_GENERATOR_DIALOG_triggered()
{
    auto dialog = new CPasswordGenerateDialog(this);
    dialog->exec();
    safe_delete(dialog);
}

void CWebPassWareMainWindow::on_ACTION_SETTINGS_DIALOG_triggered(void)
{
    auto sw = new SettingsWidget(this);
    auto dialog = new SettingsDialog(sw, this);
    dialog->exec();
    dialog->close();
    safe_delete(dialog)
}

