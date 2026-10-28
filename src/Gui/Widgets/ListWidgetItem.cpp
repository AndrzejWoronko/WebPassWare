#include "ListWidgetItem.h"

namespace wpw {

CListWidgetItem::CListWidgetItem(QListWidget *parent) : QListWidgetItem(parent)
{

}

CListWidgetItem::CListWidgetItem(const QString &text, QListWidget *parent) : QListWidgetItem(text, parent)
{

}

} // namespace wpw
