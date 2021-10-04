#ifndef CTABWIDGET_H
#define CTABWIDGET_H

#include "Global.h"
#include <QTabWidget>
#include <QTabBar>

class CTabWidget : public QTabWidget
{

    Q_OBJECT
public:
    explicit CTabWidget(QWidget *parent = NULL);
    ~CTabWidget();
    int addTabWidget(QWidget *w, const QString &label = QString(), const QString &iconName = QString());
    void insertTabWidget(int index, QWidget *w, const QString &label = QString(), const QString &iconName = QString());
    void hideTabButton(int index = 0);
    void addAndSetTabWidget(QWidget *w, const QString &label = QString(), const QString &iconName = QString());

//Dodanie kontrolerów
    int addTabWidgetController(QWidget *w, QWidget *w_ctrl, const QString &label = QString(), const QString &iconName = QString());
    void insertTabWidgetController(int index, QWidget *w,QWidget *w_ctrl, const QString &label = QString(), const QString &iconName = QString());
    void addAndSetTabWidgetController(QWidget *w, QWidget *w_ctrl, const QString &label = QString(), const QString &iconName = QString());



protected:
  //virtual void tabInserted(int index);

private:

    QHash<QWidget*,QWidget*> m_widget_list;  //Tablica widgetami  i controller  widgeta
signals:

public slots:

    void closeTab(int index);
    void showTab(int index);

};

#endif // CTABWIDGET_H
