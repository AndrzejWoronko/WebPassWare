#include "AbstractMainWindow.h"
#include "MessageBox.h"
#include "Style.h"

CAbstractMainWindow::CAbstractMainWindow(const QString &name,QWidget *parent) :
        QMainWindow(parent), CAbstractDialogStateManager(), m_main_window_name(name)
{
    m_actions.clear();
    m_menus.clear();
    initMainWidgets();
}

CAbstractMainWindow::~CAbstractMainWindow()
{

}

void CAbstractMainWindow::initMainWidgets()
{
// Dodanie podstawowych właściowości
  initActions();     // Akcje
  initInterface();   // Pola
  //initConnections(); //Połącznia pól
}

void CAbstractMainWindow::initActions()
{
    CAction *actionExit = new CAction(tr("E&xit"), ICON("Exit"), tr("Exit"), QString("Ctrl+X"), QString("ACTION_EXIT"), this);
    m_actions.insert(actionExit->getActionName(), actionExit);
    CAction *actionAbout = new CAction(tr("Ab&out QT"), ICON("Qt"), tr("About QT"), QString("Ctrl+O"), QString("ACTION_ABOUT_QT"), this);
    m_actions.insert(actionAbout->getActionName(), actionAbout);
}

void CAbstractMainWindow::initInterface()
{
    //Central widget
    m_centralwidget = new QWidget(this);
    //Menu bar
    m_menuBar = new QMenuBar(this);
    m_menuBar->setGeometry(QRect(0, 0, 850, 25));
    this->setMenuBar(m_menuBar);
    //Status bar
    m_statusBar = new QStatusBar(this);
    this->setStatusBar(m_statusBar);
    m_statusBar->setMaximumHeight(35);
    m_statusBar->setMinimumHeight(35);
    //Tool bar
    m_toolBar = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, m_toolBar);
    this->setCentralWidget(m_centralwidget);
}

void CAbstractMainWindow::setAdditionalWidgets()
{
    setActions();
    setInterface();
    setMenu();

//Dodanie z listy menu do menuBar
    Q_FOREACH(auto m, this->m_menus)
        m_menuBar->addAction(m->menuAction());

    setToolBar();
    setConnections();
    QMetaObject::connectSlotsByName(this);
    restoreDialogState();
}

void CAbstractMainWindow::centerAndResize()
{
    QScreen *screen = QGuiApplication::screens().first();
    if (screen)
    {
        // Pobranie wymiarów ekranu
        //QSize availableSize = qApp->desktop()->availableGeometry().size();
        QSize availableSize = screen->availableSize();

        int width  = availableSize.width();
        int height = availableSize.height();
        DEBUG_WITH_LINE << "Dostepne wymiary " << width << "x" << height;
        width  *= 0.9;   // 90% ekranu
        height *= 0.9;  // 90% ekranu
        DEBUG_WITH_LINE << "Wyliczone wymiary" << width << "x" << height;
        QSize newSize( width, height );
        //setGeometry(QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, newSize, qApp->desktop()->availableGeometry()));
        setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, newSize, screen->availableGeometry()));
    }
}

void CAbstractMainWindow::setIconName(const QString &iconName)
{
    if (CStyle::iconFromStyleExist(iconName))
        this->setWindowIcon(CStyle::iconFromStyle(iconName));
}

CAction *CAbstractMainWindow::getAction(const QString &actionName)
{
    return m_actions.value(actionName);
}

QHash <QString, CAction*> CAbstractMainWindow::getActionsList()
{
    return m_actions;
}

void CAbstractMainWindow::closeEvent(QCloseEvent *event)
{
    saveDialogState();
    QMainWindow::closeEvent(event);
}

void CAbstractMainWindow::restoreDialogState()
{
    QByteArray state_g = m_dialogState->getState(QString("%1Geometry").arg(m_main_window_name));
    QByteArray state_s = m_dialogState->getState(QString("%1State").arg(m_main_window_name));

    this->restoreGeometry(state_g);
    this->restoreState(state_s);
}

void CAbstractMainWindow::saveDialogState()
{
    QByteArray state = this->saveGeometry();
    QByteArray oldState = m_dialogState->getState(QString("%1Geometry").arg(m_main_window_name));

    if(state != oldState)
        m_dialogState->saveState(QString("%1Geometry").arg(m_main_window_name), state);
    state = this->saveState();
    oldState = m_dialogState->getState(QString("%1State").arg(m_main_window_name));
    if(state != oldState)
        m_dialogState->saveState(QString("%1State").arg(m_main_window_name), state);
}

void CAbstractMainWindow::on_ACTION_ABOUT_QT_triggered()
{
    CMessageBox::AboutQtDialog(this);
}

void CAbstractMainWindow::on_ACTION_EXIT_triggered()
{
    saveDialogState();
    close();
}
