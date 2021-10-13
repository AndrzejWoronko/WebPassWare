#include "FormValidator.h"

CFormValidator::CFormValidator(FormFieldHashList fields)
{     
   m_fields = fields;
   m_validateCallback = NULL;
   m_validateEmpty = NULL;
}

CFormValidator::~CFormValidator()
{

}

void CFormValidator::setValidateCallback(validateFuncHash callback_func)
{
    m_validateCallback = callback_func;
}

void CFormValidator::setValidateEmpty(validateFuncHash empty_func)
{
    m_validateEmpty = empty_func;
}

QString CFormValidator::validateForm(void)
{
    QString error = QString("");

    if (m_validateCallback)
        error = m_validateCallback(m_fields);

    return error;
}

bool CFormValidator::isEmptyForm(void)
{
    QString error = QString("");

    if (m_validateEmpty)
      {
         error = m_validateEmpty(m_fields);
         if (error.length())
             return true;
         else
             return false;
      }
    else
         return false;
}
