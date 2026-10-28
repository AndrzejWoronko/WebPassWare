#ifndef PASSWORDFORMATDELEGATE_H
#define PASSWORDFORMATDELEGATE_H

#include "Global.h"
#include <QStyledItemDelegate>
#include <QAbstractItemModel>

namespace wpw {

class PasswordFormatDelegate : public QStyledItemDelegate
{

public:
    PasswordFormatDelegate(QObject *parent = nullptr);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

} // namespace wpw

#endif // PASSWORDFORMATDELEGATE_H
