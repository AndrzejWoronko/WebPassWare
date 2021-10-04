#ifndef BUTTON_H
#define BUTTON_H

#include "Global.h"
#include <QPushButton>
#include <QToolButton>
#include <QAction>

class CButtonPrivate
{
    ADD_FIELD(QString, m_text, getText, setText)
    ADD_FIELD(QString, m_icon, getIcon, setIcon)
    ADD_FIELD(QSize, m_shape, getShape, setShape)
    ADD_FIELD(QString, m_tipText, getTipText, setTipText)
public:
    CButtonPrivate(const QString &text, const QString &tipText = QString(), const QString &icon = QString(), QSize shape = QSize());
    CButtonPrivate();
    bool isNullIcon();
};


//Usage  CButton(CButtonPrivate("text button","text tip","icon_name",QSize))
//Button action slot name
// on_BUTTON_NAME_clicked()

class CButton : public QPushButton
{
    Q_OBJECT

    ADD_FIELD(QString, m_buttonName, getName, setName)
    ADD_PTR_FIELD(QAction, m_action, getAction, setAction)

private:
    CButtonPrivate m_structure;
    void setOptions();

public:
    CButton(CButtonPrivate structure, QWidget *parent = NULL, QSize iconSize = QSize());
    void setIconPolicy(const QString &iconName, QSize iconSize = QSize());
    void setIconFromTheme(const QString &iconName);
    QString getText(void);
};

#endif // BUTTON_H
