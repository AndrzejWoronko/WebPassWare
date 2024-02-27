#include "ToggleSwitch.h"
#include "Application.h"
#include <QMouseEvent>
#include <QPainter>

static const int s_height = 26;
static const int s_innerMargin = 4;
static const int s_handleSize = s_height - s_innerMargin * 2;
static const int s_width = s_handleSize * 2 + s_innerMargin * 2;

CToggleSwitch::CToggleSwitch(QWidget *parent): QWidget{parent}
{
    setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed}); // sizeHint is the only acceptable size
    setFocusPolicy(Qt::TabFocus); // can tab into the widget
    setAttribute(Qt::WA_Hover); // repaint on mouse-enter/mouse-exit
}

void CToggleSwitch::setChecked(bool checked)
{
    if (!m_checkable)
        return;

    if (m_checked == checked)
        return;
    m_checked = checked;
    emit toggled(checked);
    update();
}

bool CToggleSwitch::isChecked() const
{
    return m_checked;
}

void CToggleSwitch::setCheckable(bool checkable)
{
    m_checkable = checkable;
}

void CToggleSwitch::toggle()
{
    setChecked(!m_checked);
}

QSize CToggleSwitch::sizeHint() const
{
    return QSize(s_width, s_height);
}

QBrush CToggleSwitch::hoverOutlineBrush(const QRect &rect)
{
    // Instructions from the designer:
    // "Draw line passing by center of rectangle (+4% to the right)
    // and that is perpendicular to the topleft-bottomright diagonal.
    // This line intersects the top and bottom in two points, which are the gradient stops"

    const qreal w = rect.width();
    const qreal h = rect.height();
    const qreal xmid = w * 0.54;
    const qreal xoffset = (h * h) / (2 * w); // Proportionality: xoffset / (h/2) = h / w
    const qreal x0 = xmid - xoffset;
    const qreal x1 = xmid + xoffset;

    QLinearGradient gradient(x0, h, x1, 0);
    gradient.setColorAt(0.0, QColor(0x53, 0x94, 0x9f));
    gradient.setColorAt(1.0, QColor(0x75, 0x55, 0x79));
    return QBrush(gradient);
}

QBrush CToggleSwitch::progressBarOutlineBrush(const QRect &rect)
{
    QLinearGradient gradient(0, rect.height(), rect.width(), 0);
    // gradient.setColorAt(0.0, QColor(0x11, 0xc2, 0xe1));
    // gradient.setColorAt(1.0, QColor(0x89, 0x3a, 0x94));

    gradient.setColorAt(0.0, QColor(Qt::green));
    gradient.setColorAt(1.0, QColor(Qt::darkGreen));

    return QBrush(gradient);
//  return QBrush(QColor(Qt::green));
}

QColor CToggleSwitch::pressedOutlineColor()
{
    return QColor(0x32, 0x2d, 0x35);
}

void CToggleSwitch::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPalette palete = APPI->palette();

    // Another advantage of QStyle: custom widgets can share code
    // with it (here just the color repository, but we could share more code
    // if needed).
    // With QSS, we can't use QSS to style our custom widget.

    // Similar colors as for pushbuttons (but the shape is different)
    if (!isEnabled())
    {
        painter.setPen( palete.color(QPalette::Button) /*ColorRepository::buttonOutlineColor()*/);
        painter.setOpacity(0.5);
    }
    else if (m_mouseDown) // Sunken
        painter.setPen(pressedOutlineColor());
    else if (underMouse() || hasFocus())
        painter.setPen(QPen(hoverOutlineBrush(rect()), 1));
    else
        painter.setPen(palete.color(QPalette::Button) /*ColorRepository::buttonOutlineColor()*/);

    if (m_checked)
        painter.setBrush(palete.color(QPalette::Background) /*ColorRepository::baseBackground()*/);
    const qreal radius = height() / 2;
    painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), radius, radius);

    // Now draw the handle

    QRect valueRect = rect().adjusted(s_innerMargin, s_innerMargin, -s_innerMargin, -s_innerMargin);
    valueRect.setWidth(valueRect.height()); // must be a square

    if (m_checked) {
        valueRect.moveLeft(width() / 2);
        painter.setPen(QPen(progressBarOutlineBrush(valueRect), 1));
        painter.setBrush(progressBarOutlineBrush(valueRect));
        //painter.setPen(QColor(Qt::green));
        //painter.setBrush(QColor(Qt::green));
    } else {
        painter.setBrush(palete.color(QPalette::Background) /*ColorRepository::baseBackground()*/);
    }
    painter.drawEllipse(valueRect);
}

void CToggleSwitch::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mouseDown = true;
    } else {
        event->ignore();
    }
}

void CToggleSwitch::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_mouseDown) {
        m_mouseDown = false;
        toggle();
        emit checked(m_checked);
    } else {
        event->ignore();
    }
}

void CToggleSwitch::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        toggle();
        emit checked(m_checked);
    } else {
        event->ignore(); // let it propagate to the parent (e.g. so that Return closes dialogs)
    }
}
