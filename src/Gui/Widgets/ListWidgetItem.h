#ifndef CLISTWIDGETITEM_H
#define CLISTWIDGETITEM_H

#include "Global.h"
#include <QListWidgetItem>

class CListWidgetItem : public QListWidgetItem
{
    ADD_FIELD(QString, m_actionName, getActionName, setActionName)

public:
    explicit CListWidgetItem(QListWidget *parent = NULL);
    explicit CListWidgetItem(const QString &text, QListWidget *parent = NULL);
};

#endif // CLISTWIDGETITEM_H
