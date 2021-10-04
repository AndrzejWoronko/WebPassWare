#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include "Global.h"
#include <QToolButton>
#include "Button.h"


class CToolButton : public QToolButton
{
    Q_OBJECT

    ADD_FIELD(QString, m_buttonName, getName, setName)
    ADD_PTR_FIELD(QAction, m_action, getAction, setAction)

private:
    CButtonPrivate m_structure;
    void setOptions();

public:
    CToolButton(CButtonPrivate structure, QWidget *parent = NULL, QSize iconSize = QSize());
    void setIconPolicy(const QString &iconName, QSize iconSize = QSize());
    void setIconFromTheme(const QString &iconName);
    void setTopBottomLayout();
    QString getText(void);
};

#endif // TOOLBUTTON_H
