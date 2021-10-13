#include "DateEdit.h"

CDateEdit::CDateEdit(QWidget * parent) : QDateEdit(parent)
{
    this->setDate(QDate::currentDate());
    this->setDisplayFormat(DATE_FORMAT);
    this->setCalendarPopup(true);
    this->setCalendarWidget(new CCalendarWidget());
}

CDateEdit::CDateEdit(const QDate &date, QWidget * parent) : QDateEdit(date, parent)
{
    this->setDisplayFormat(DATE_FORMAT);
    this->setCalendarPopup(true);
    this->setCalendarWidget(new CCalendarWidget());
}

CDateTimeEdit::CDateTimeEdit(QWidget * parent) : QDateTimeEdit(parent)
{
    this->setDate(QDate::currentDate());
    this->setDisplayFormat(DATETIME_FORMAT_SHORT);
    this->setCalendarPopup(true);
    this->setCalendarWidget(new CCalendarWidget());
}

CDateTimeEdit::CDateTimeEdit(const QDateTime &date, QWidget * parent) : QDateTimeEdit(date, parent)
{
    this->setDisplayFormat(DATETIME_FORMAT_SHORT);
    this->setCalendarPopup(true);
    this->setCalendarWidget(new CCalendarWidget());
}

CMonthEdit::CMonthEdit(QWidget * parent) : CDateEdit(parent)
{
    this->setDisplayFormat(MONTH_FORMAT);
    connect(this, SIGNAL(dateChanged(const QDate &)), this, SLOT(setMonth(const QDate &)));
}

CMonthEdit::CMonthEdit(const QDate &date, QWidget * parent ) : CDateEdit(date, parent)
{    
    this->setDisplayFormat(MONTH_FORMAT);
    this->setMonth(date);
    connect(this, SIGNAL(dateChanged(const QDate &)), this, SLOT(setMonth(const QDate &)));
}

void CMonthEdit::setMonth(const QDate &date)
{
     QDate d = date;
     blockSignals(true);
     d.setDate(date.year(), date.month(), 1);
     this->setDate(d);
     blockSignals(false);
     DEBUG_WITH_LINE << "Month changed: " << d;
     emit monthChanged(d);
}
