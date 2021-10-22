#ifndef CSERVICE_H
#define CSERVICE_H

#include "Global.h"
#include "Exception.h"

class CService
{
public:
    CService();

protected:
    static void showErrorInfo(CException *e);
};

#endif // CSERVICE_H
