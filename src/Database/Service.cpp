#include "Service.h"

CService::CService()
{

}

void CService::clearError()
{
    m_error.clear();
}

void CService::showErrorInfo(CException *e)
{    
    DEBUG_WITH_LINE << "Error in method: "  << e->getSourceMethod();
    DEBUG_WITH_LINE << "Message: " << e->getMessage();
    this->setError(e->getMessage());
}
