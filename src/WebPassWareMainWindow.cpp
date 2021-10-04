#include "WebPassWareMainWindow.h"

CWebPassWareMainWindow::CWebPassWareMainWindow(QWidget *parent) : CAbstractMainWindow(QString("WebPassWareMainWindow"), parent)
{
    setAdditionalWidgets();
}

CWebPassWareMainWindow::~CWebPassWareMainWindow()
{

}

void CWebPassWareMainWindow::setInterface(void)
{

}

void CWebPassWareMainWindow::setActions(void)
{

    CAction *actionHelp = new CAction(tr("&O programie"), ICON("About"), tr("O programie"), QString("Ctrl+H"), QString("ACTION_HELP"), this);
    m_actions.insert(actionHelp->getActionName(), actionHelp);
}

void CWebPassWareMainWindow::setMenu(void)
{
    QMenu *fileMenu;
    QMenu *helpMenu;

    fileMenu = new QMenu(m_menuBar);
    fileMenu->setTitle(tr("&Plik"));
    fileMenu->addAction(m_actions.value(QString("ACTION_REFRESH_TABLE_LIST")));
    fileMenu->addSeparator();
    fileMenu->addAction(m_actions.value(QString("ACTION_EXIT")));

    helpMenu = new QMenu(m_menuBar);
    helpMenu->setTitle(tr("&Pomoc"));
    helpMenu->addAction(m_actions.value(QString("ACTION_HELP")));
    helpMenu->addAction(m_actions.value(QString("ACTION_VISIT_WEBSITE")));
    helpMenu->addAction(m_actions.value(QString("ACTION_ABOUT_QT")));

    m_menus << fileMenu << helpMenu;
    return;
}

void CWebPassWareMainWindow::setToolBar(void)
{
    m_toolBar->addAction(m_actions.value(QString("ACTION_REFRESH_TABLE_LIST")));
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_actions.value(QString("ACTION_EXIT")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_HELP")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_VISIT_WEBSITE")));
    m_toolBar->addAction(m_actions.value(QString("ACTION_ABOUT_QT")));
    return;
}

void CWebPassWareMainWindow::setConnections(void)
{

}

void CWebPassWareMainWindow::refreshInfo()
{
//    m_dbLabel->setText(tr("Baza: %1, host: %2, baza: %3").arg(DB.getMBaseType()).arg(DB.getMBaseHost()).arg(DB.getMBaseName()));
//    m_dateLabel->setText(tr("Data: <b>%1</b> %2").arg(QDate::currentDate().toString(DATE_FORMAT)).arg(QTime::currentTime().toString(TIME_FORMAT)));
    QTimer::singleShot(1000, this, SLOT(refreshInfo()));
}

