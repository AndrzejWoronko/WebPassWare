#ifndef CFONTEDIT_H
#define CFONTEDIT_H

#include "Global.h"
#include "Style.h"
#include "Layouts.h"

#include <QLabel>
#include <QToolButton>

class CFontEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QFont m_font READ getFont WRITE setFont NOTIFY fontChanged)
    ADD_PTR_PROPERTY(CHBoxLayout, m_layout, getLayout)
    ADD_PTR_PROPERTY(QLabel, m_label, getLabel)
    ADD_PTR_PROPERTY(QToolButton, m_button, getButton)

public:
    explicit CFontEdit(QWidget *parent = NULL);
    ~CFontEdit();
    QFont getFont() const;
    void setToolTip(const QString &text);
    void setFontSize(int size);

public slots:
    void setFont(const QFont &arg);

private:
    void initConnections();
    void updateFont();

    QFont m_font;

private slots:
    void browse();

signals:
    void fontChanged(const QFont& font);
    void fontSizeChanged(int size);
};

#endif // CFONTEDIT_H
