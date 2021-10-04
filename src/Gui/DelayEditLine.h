#ifndef DELAYEDITLINE_H
#define DELAYEDITLINE_H

#include "Global.h"
#include <QLineEdit>
#include <QStyle>

#define DELAY_EDIT_LINE_MS    300

class CDelayEditLine : public QLineEdit
{
    Q_OBJECT

    QTimer *m_typingTimer;

public:
    CDelayEditLine(QWidget *parent = NULL);
    ~CDelayEditLine();

signals:

    void delayEditingFinished(const QString &);

private slots:

    void onTextEdited(const QString & newText);
    void delayEditingFinish();
};

#endif // DELAYEDITLINE_H
