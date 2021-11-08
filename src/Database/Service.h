#ifndef CSERVICE_H
#define CSERVICE_H

#include "Global.h"
#include "Exception.h"

class CService
{
    ADD_FIELD(QString, m_error, getError, setError)

public:
    CService();
    void clearError();

protected:
    void showErrorInfo(CException *e);
};

#endif // CSERVICE_H
