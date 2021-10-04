#ifndef CSPLITTER_H
#define CSPLITTER_H

#include <Global.h>
#include <QSplitter>
#include "AbstractSplitterStateManager.h"

class CSplitter : public QSplitter, public CAbstractSplitterStateManager
{
    ADD_FIELD(QString, m_spitter_name, getSplitterName, setSplitterName)

public:
    explicit CSplitter(const QString &splitterName, Qt::Orientation orientation, QWidget *parent = NULL);
    explicit CSplitter(Qt::Orientation orientation, QWidget *parent = NULL);
    ~CSplitter();

    virtual void restoreSplitterState();
    virtual void saveSplitterState();
};

#endif // CSPLITTER_H
