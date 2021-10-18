#ifndef CEXCEPTION_H
#define CEXCEPTION_H

#include "Global.h"

class CException
{
    ADD_FIELD(QString, m_komunikat, getMessage, setMessage)
    ADD_FIELD(QString, m_metoda, getSourceMethod, setSourceMethod)
public:
    CException();
    CException(const QString &metoda, const QString &kom = QString(""));
};

#endif // CEXCEPTION_H
