#ifndef DATEEDIT_H
#define DATEEDIT_H

#include "Global.h"
#include <QDateEdit>
#include <QDateTimeEdit>
#include "CalendarWidget.h"

class CDateEdit : public QDateEdit
{
    Q_OBJECT
public:
    CDateEdit(QWidget  *parent = nullptr);
    CDateEdit(const QDate &date = QDate::currentDate(), QWidget  *parent = nullptr);
};


class CDateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    CDateTimeEdit(QWidget  *parent = nullptr);
    CDateTimeEdit(const QDateTime &date = QDateTime::currentDateTime(), QWidget  *parent = nullptr);
};

class CMonthEdit : public CDateEdit
{
    Q_OBJECT
public:
    CMonthEdit(QWidget  *parent = nullptr);
    CMonthEdit(const QDate &date = QDate::currentDate(), QWidget  *parent = nullptr);

signals:
    void monthChanged(const QDate &);

public slots:
    void setMonth(const QDate &date);
};


#endif // CDATEEDIT_H
