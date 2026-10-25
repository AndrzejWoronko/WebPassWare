#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QCalendarWidget>

class CCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    CCalendarWidget(QWidget  *parent = nullptr);
};

#endif // CCALENDARWIDGET_H
