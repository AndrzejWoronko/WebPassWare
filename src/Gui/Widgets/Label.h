#ifndef CLABEL_H
#define CLABEL_H

#include "Global.h"
#include <QLabel>
#include "Style.h"

namespace wpw {

class CLabel : public QLabel
{

    QString m_text;

public:
    CLabel(const QString &text =  QString(), const QString &pixmap = QString(), QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CLabel() { DEBUG_WITH_LINE << "~dtor "; }

    void setPixmap(const QString &pixmap);
    void setBold(void);
};

} // namespace wpw

#endif // CLABEL_H
