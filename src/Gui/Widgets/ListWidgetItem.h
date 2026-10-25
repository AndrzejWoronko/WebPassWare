#ifndef CLISTWIDGETITEM_H
#define CLISTWIDGETITEM_H

#include "Global.h"
#include <QListWidgetItem>

class CListWidgetItem : public QListWidgetItem
{
    ADD_FIELD(QString, m_actionName, getActionName, setActionName)

public:
    explicit CListWidgetItem(QListWidget *parent = nullptr);
    explicit CListWidgetItem(const QString &text, QListWidget *parent = nullptr);
};

#endif // CLISTWIDGETITEM_H
