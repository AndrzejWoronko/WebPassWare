#ifndef CTOGGLESWITCH_H
#define CTOGGLESWITCH_H

#include "Global.h"
#include <QWidget>

class CToggleSwitch : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled)
public:
    explicit CToggleSwitch(QWidget *parent = nullptr);

    void setCheckable(bool checkable);
    void setChecked(bool checked);
    bool isChecked() const;

    void toggle();

    QSize sizeHint() const override;

signals:
    void checked(bool checked); // by user
    void toggled(bool checked); // by user or by program

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QBrush hoverOutlineBrush(const QRect &rect);
    QBrush progressBarOutlineBrush(const QRect &rect);
    QColor pressedOutlineColor();

    bool m_checked = false;
    bool m_mouseDown = false;
    bool m_checkable = true;
};

#endif // CTOGGLESWITCH_H
