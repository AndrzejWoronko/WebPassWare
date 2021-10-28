#ifndef CLISTWIDGET_H
#define CLISTWIDGET_H


#include "Global.h"
#include "Action.h"
#include "ListWidgetItem.h"
#include <QListWidget>
#include <QDragMoveEvent>

class CListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit CListWidget(QWidget *parent = NULL);
    void addItem(const QString &text, const QString &status = QString(), const QString &iconName = QString(), QSize size = QSize());
    void addAction(CAction *action, const QString &status = QString(), QSize size = QSize());
    void addItemWidget(QWidget *w);

};
#endif // CLISTWIDGET_H
