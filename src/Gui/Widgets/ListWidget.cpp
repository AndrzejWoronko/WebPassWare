#include "ListWidget.h"
#include "Style.h"


CListWidget::CListWidget(QWidget *parent) : QListWidget(parent)
{
    this->setMouseTracking(true);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
}

void CListWidget::addItem(const QString &text, const QString &status, const QString &iconName, QSize size)
{
    QString status_tmp = status;
    CListWidgetItem *item = new CListWidgetItem(text, this);

    if (CStyle::iconFromStyleExist(iconName))
        item->setIcon(CStyle::iconFromStyle(iconName));
    else
        item->setIcon(QIcon::fromTheme(iconName));

    //item->setText(text);
    item->setToolTip(status_tmp.remove("&"));
    item->setStatusTip(status_tmp.remove("&"));
    item->setSizeHint(size);
    QListWidget::addItem(item);
}

void CListWidget::addAction(CAction *action, const QString &status, QSize size)
{
    QString status_tmp = status;
    CListWidgetItem *item = new CListWidgetItem(this);

    item->setActionName(action->getActionName());

    if (CStyle::iconFromStyleExist(action->getIconName()))
        item->setIcon(CStyle::iconFromStyle(action->getIconName()));
    else
        item->setIcon(QIcon::fromTheme(action->getIconName()));

    item->setText(action->getText().remove("&"));
    item->setToolTip(status_tmp.remove("&"));
    item->setStatusTip(status_tmp.remove("&"));
    item->setSizeHint(size);
    QListWidget::addItem(item);
}

void CListWidget::addItemWidget(QWidget *w)
{
    CListWidgetItem *item = new CListWidgetItem(this);

    item->setSizeHint(w->size());
    QListWidget::addItem(item);
    QListWidget::setItemWidget(item, w);
}
