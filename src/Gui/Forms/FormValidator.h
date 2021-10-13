#ifndef FORMVALIDATOR_H
#define FORMVALIDATOR_H

#include "Global.h"
#include "FormAbstractField.h"
#include "Form.h"

class CFormValidator
{
public:
    CFormValidator(FormFieldHashList fields);
    virtual ~CFormValidator();

    void setValidateCallback(validateFuncHash callback_func);
    void setValidateEmpty(validateFuncHash empty_func);
    QString validateForm(void);
    bool isEmptyForm(void);

private:
    FormFieldHashList m_fields;
    validateFuncHash m_validateCallback;
    validateFuncHash m_validateEmpty;
};

#endif // FORMVALIDATOR_H
