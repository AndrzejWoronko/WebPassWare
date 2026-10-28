#ifndef CABSTRACTFORMWIDGET_H
#define CABSTRACTFORMWIDGET_H

#include "Global.h"
#include "Form.h"
#include "Label.h"

/**
 * @brief The CAbstractFormWidget class - klasa abstrakcyjna wyświetlania formatki jako QWidget
 */

namespace wpw {

class CAbstractFormWidget : public QWidget
{
    Q_OBJECT

    ADD_PTR_PROPERTY(CGridLayout, m_formLayout, getFormLayout) //layout widget

public:

    CAbstractFormWidget();
    explicit CAbstractFormWidget(QWidget *parent = nullptr);
    ~CAbstractFormWidget();

    void addFieldsAndComponents();

    FormFieldHashList getFields();

    void addField(CFormAbstractField *f);

    CAbstractFormWidget *getWidget();

private:

    FormFieldHashList m_fields;

public:

    //Virtualne funkcje do dodawnia komponentów i pól
    virtual void addComponents() = 0;
    virtual void addFields() = 0;

};

} // namespace wpw

#endif // CABSTRACTFORMWIDGET_H
