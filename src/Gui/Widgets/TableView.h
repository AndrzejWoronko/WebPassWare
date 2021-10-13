#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "Global.h"
//#include <QtSql>
#include <QKeyEvent>
#include <QTableView>
#include <QModelIndex>
#include <QHeaderView>
#include <QContextMenuEvent>

class CTableView :public QTableView
{
    Q_OBJECT

public:
    CTableView( QWidget  *parent = NULL);
    ~CTableView();

    bool eventFilter(QObject *obj, QEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void setAutoResizeColumn(int column);
    virtual void setupVerticalHeader(double rowHeight = -1.0);
    virtual void setModel(QAbstractItemModel *model);

protected:
//    void contextMenuEvent(QContextMenuEvent *event);

private:
    /**
      * @brief którą kolumnę rozszerzać przy zmianie rozmiaru tabeli, -1 wyłącza tę opcję
      */
    int m_autoResizeColumn;

protected:
    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);

public slots:
    virtual void resizeColumnsToContents();
    void goFirst();
    void goLast();
    void goNext();
    void goPrev();

signals:
    void currentIndexChanged(const QModelIndex &current, const QModelIndex &previous);
    //void refreshView();
    void keyInsert();
    void keyDelete();
    void keyEnter();
    void selectedAll();
    void selectedOne(const QModelIndex &index);
    void keyFind();
    void keyF1();
    void keyF2();
    void keyF3();
    void keyF4();
    void keyF5();
    void keyF6();
    void keyF7();
    void keyF8();
    void keyF9();
    void keyF10();
    void keyF11();
    void keyF12();
};

#endif // TABLEVIEW_H
