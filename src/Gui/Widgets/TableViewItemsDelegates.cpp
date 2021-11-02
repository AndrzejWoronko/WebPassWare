#include "TableViewItemsDelegates.h"
#include <QPainter>

PasswordFormatDelegate::PasswordFormatDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void PasswordFormatDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant var = index.model()->data(index);

    if (!var.toString().isEmpty())
    {
        QString pass  = QString().fill('*', var.toString().length());
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        opt.widget->style()->drawItemText(painter, opt.rect, Qt::AlignLeft, opt.palette, true, pass);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}
