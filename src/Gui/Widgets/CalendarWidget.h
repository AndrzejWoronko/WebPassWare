#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QCalendarWidget>

namespace wpw {

class CCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    CCalendarWidget(QWidget  *parent = nullptr);
};

} // namespace wpw

#endif // CCALENDARWIDGET_H
