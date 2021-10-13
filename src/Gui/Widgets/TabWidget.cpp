#include "TabWidget.h"
#include "Style.h"

CTabWidget::CTabWidget(QWidget *parent) : QTabWidget(parent)
{
    m_widget_list.clear();

    this->setWindowTitle(tr("Pasek zakładek"));
    this->setUsesScrollButtons(true);
    this->setDocumentMode(true);
    this->setTabsClosable(true);
    this->setMovable(true);
    if (this->tabBar())
    {
        this->tabBar()->setShape(QTabBar::RoundedNorth);
        this->tabBar()->setSelectionBehaviorOnRemove(QTabBar::SelectLeftTab);
        this->tabBar()->setFocusPolicy(Qt::TabFocus);
    }
    //this->tabBar()->setMinimumWidth(200);
    this->setStyleSheet("QTabBar::tab { min-width: 100px; }");

    //this->tabBar()->setShape(QTabBar::RoundedSouth);
    //this->setUsesScrollButtons(true);
    //this->tabBar()->setShape(QTabBar::TriangularNorth);
    //this->setTabsClosable(true);
    //this->setShape(QTabBar::RoundedSouth);
    //this->addDockWidget(Qt::TopDockWidgetArea, tabDock);
    //this->setElideMode(Qt::ElideRight);
    //this->setFocusPolicy(Qt::TabFocus);    
}

void CTabWidget::closeTab(int index)
{
    //Wystarczy skasować tylko widget i zamyka się tab
    //po zamknięciu robi się renumeracja indeksu
    if (m_widget_list.contains(widget(index)))
       {
            delete m_widget_list.value(widget(index));  //Kasujemy controller który kasuję również widget`a
            m_widget_list.remove(widget(index));
       }
    else
        delete widget(index);
}

void CTabWidget::showTab(int index)
{
    this->setCurrentIndex(index);
    //m_tabWidget->setTabText(index, QString(tr("Tab %1").arg(index)));
    //QWidget *tab = m_tabWidget->widget(index);
    //m_tabWidget->setCurrentWidget(tab);
    //emit m_tabWidget->tabBarClicked(index);
}

CTabWidget::~CTabWidget()
{
   //  delete all widgets
   for (int i = 0; i < this->count(); i++)
    {
       if (m_widget_list.contains(this->widget(i)))
          delete m_widget_list.value(this->widget(i));  //Kasujemy controller który kasuję również widget`a
       else
          delete this->widget(i);
    }
}

void CTabWidget::hideTabButton(int index)
{
    if (this->tabBar())
       {
          if (this->tabBar()->tabButton(index, QTabBar::RightSide))
             this->tabBar()->tabButton(index, QTabBar::RightSide)->resize(0, 0);
          if (this->tabBar()->tabButton(index, QTabBar::LeftSide))
             this->tabBar()->tabButton(index, QTabBar::LeftSide)->resize(0, 0);
       }
}

int CTabWidget::addTabWidget(QWidget *w, const QString &label, const QString &iconName)
{
    int index;

    if (!iconName.isEmpty())
        index  = this->addTab(w, CStyle::iconFromStyle(iconName), label);
    else
        index  = this->addTab(w, label);

    setTabText(index, label);
    emit this->currentChanged(index);    

    return index;
}

void CTabWidget::insertTabWidget(int index, QWidget *w, const QString &label, const QString &iconName)
{
    if (!iconName.isEmpty())
        this->insertTab(index, w, CStyle::iconFromStyle(iconName), label);
    else
        this->insertTab(index, w, label);

    this->setTabText(index, label);
    emit this->currentChanged(index);    
}

void CTabWidget::addAndSetTabWidget(QWidget *w, const QString &label, const QString &iconName)
{
    int tab_idx=-1;
    tab_idx = this->addTabWidget(w, label, iconName);
    setTabText(tab_idx, QString(label));
}

int CTabWidget::addTabWidgetController(QWidget *w, QWidget *w_ctrl, const QString &label, const QString &iconName)
{
    int index;

    if (!iconName.isEmpty())
        index  = this->addTab(w, CStyle::iconFromStyle(iconName), label);
    else
        index  = this->addTab(w, label);

    m_widget_list.insert(w, w_ctrl); //Dodanie do listy widoków;
    setTabText(index, label);
    emit this->currentChanged(index);    

    return index;
}

void CTabWidget::insertTabWidgetController(int index, QWidget *w, QWidget *w_ctrl, const QString &label, const QString &iconName)
{
    if (!iconName.isEmpty())
        this->insertTab(index, w, CStyle::iconFromStyle(iconName), label);
    else
        this->insertTab(index, w, label);

    m_widget_list.insert(w, w_ctrl); //Dodanie do listy widoków;
    this->setTabText(index, label);
    emit this->currentChanged(index);    
}

void CTabWidget::addAndSetTabWidgetController(QWidget *w, QWidget *w_ctrl, const QString &label, const QString &iconName)
{
    int tab_idx=-1;
    tab_idx = this->addTabWidgetController(w, w_ctrl, label, iconName);
    setTabText(tab_idx, QString(label));
}
