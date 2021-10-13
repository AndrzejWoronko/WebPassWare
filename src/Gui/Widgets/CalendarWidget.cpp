#include "CalendarWidget.h"

CCalendarWidget::CCalendarWidget(QWidget * parent) : QCalendarWidget(parent)
{
    this->setMinimumDate(QDate(1900, 1, 1));
    this->setMaximumDate(QDate(2099, 1, 1));
    this->setGridVisible(true);
    this->setFirstDayOfWeek(Qt::Monday);
    this->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
}
