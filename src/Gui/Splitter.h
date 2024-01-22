#ifndef CSPLITTER_H
#define CSPLITTER_H

#include <Global.h>
#include <QSplitter>
#include "AbstractSplitterStateManager.h"
#include "AbstractStateManager.h"
#include "SplitterStateManager.h"

class CSplitter : public QSplitter
{
    ADD_FIELD(QString, m_spitter_name, getSplitterName, setSplitterName)
    ADD_UNIQUE_SMART_PTR(CSplitterStateManager, m_splitter_interf, getSplitterInterf)

public:
    explicit CSplitter(const QString &splitterName, Qt::Orientation orientation, QWidget *parent = NULL);
    explicit CSplitter(Qt::Orientation orientation, QWidget *parent = NULL);
    ~CSplitter();

    void restoreSplitterState();
    void saveSplitterState();
};

#endif // CSPLITTER_H
