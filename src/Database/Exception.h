#ifndef CEXCEPTION_H
#define CEXCEPTION_H

#include "Global.h"

namespace wpw {

class CException : public std::exception
{
    ADD_FIELD(QString, m_komunikat, getMessage, setMessage)
    ADD_FIELD(QString, m_metoda, getSourceMethod, setSourceMethod)

public:
    CException();
    CException(const QString &metoda, const QString &kom = QString(""));
};

} // namespace wpw

#endif // CEXCEPTION_H
