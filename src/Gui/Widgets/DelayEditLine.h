#ifndef DELAYEDITLINE_H
#define DELAYEDITLINE_H

#include "Global.h"
#include <QLineEdit>
#include <QStyle>

constexpr int DELAY_EDIT_LINE_MS = 300;

class CDelayEditLine : public QLineEdit
{
    Q_OBJECT

    QScopedPointer<QTimer> m_typingTimer;

public:
    CDelayEditLine(QWidget *parent = NULL);
    ~CDelayEditLine() = default;

signals:

    void delayEditingFinished(const QString &);

private slots:

    void onTextEdited(const QString & newText);
    void delayEditingFinish();
};

#endif // DELAYEDITLINE_H
