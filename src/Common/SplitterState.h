#ifndef CSPLITTERSTATE_H
#define CSPLITTERSTATE_H

#include "Global.h"

class CSplitterState
{

    ADD_QSMART_PTR(QSettings, m_settings, getSettings)

public:
    CSplitterState();
    ~CSplitterState();

    void saveState(const QString &splitterName, QByteArray state);
    QByteArray getState(const QString &splitterName);

};

#endif // CSPLITTERSTATE_H
