#ifndef CLABEL_H
#define CLABEL_H

#include "Global.h"
#include <QLabel>
#include "Style.h"

class CLabel : public QLabel
{

    QString m_text;

public:
    CLabel(const QString &text =  QString(), const QString &pixmap = QString(), QWidget *parent = NULL, Qt::WindowFlags f = Qt::WindowFlags());

    void setPixmap(const QString &pixmap);
    void setBold(void);
};

#endif // CLABEL_H
