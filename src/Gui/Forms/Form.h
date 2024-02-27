#ifndef FORM_H
#define FORM_H

#include <functional>

#include "Global.h"
#include "Dialog.h"
#include "Button.h"
#include "ButtonBox.h"
#include "FormAbstractField.h"
#include "FormNumberField.h"
#include "FormButtonField.h"
#include "FormTextField.h"
#include "FormCheckField.h"
#include "FormEditorField.h"
#include "FormDateField.h"
#include "FormDateTimeField.h"
#include "FormSimpleChoiceField.h"
#include "FormSimpleIndexChoiceField.h"
#include "FormYesNoChoiceField.h"
#include "FormMonthField.h"
#include "FormRadioButtonField.h"
#include"FormToggleSwitchField.h"

  /**
   * @brief The CForm class - uniwersalna formatka do wprowadzania danych
   */

class CForm : public QWidget
{
    Q_OBJECT

    Q_ENUMS(FormMode)

    ADD_PTR(CDialog, m_dialog, getDialog)

  public:

    enum FormMode
    {
       View,
       New,
       Edit
    };


    explicit CForm(QWidget *parent = NULL);
     ~CForm();

    /**
     * @brief init - konstruujue układ okienka z dostępnych pól
     * @param fields
     */
    void init( FormFieldList fields);

    /**
     * @brief exec - pokazuje okienko
     * @param title
     * @return
     */
    bool exec(const QString &title = QString());

    /**
     * @brief isDirty - sprawdza w pętli for, czy wartość w którymś polu została zmieniona
     * @return
     */
    bool isDirty();

    //Uwaga !!! funkcja callback w pliku .h musi być static  w pliku .cpp już nie aby ją dodać
    //void setValidateCallback(std::function<QString (QList<CFormAbstractField*>)> callback);

    //Uwaga !!! funkcja callback w pliku .h musi być static  w pliku .cpp już nie aby ją dodać
    void setValidateCallback(validateFunc callback);

    /**
      Metoda służąca do ustawienia elemetów manipulacji danymi wraz z odpowiednimi dla nich labelami w naszych layoutach
      standard i additional

      @param
       layout - layout w jakim mają zostać umieszczone podane elementy
       widget - element w ktorym umieszczamy dane, badz z którego te dane wybieramy
       labelValue - string umieszczany w labelu
       row,
       col,
       dy,
       dx - wspólrzędne danych obiektów w layoucie
    */
    static void setComponents(QGridLayout *layout, QWidget *widget, const QString &labelValue, int row, int col, int dy, int dx, bool boldLabel = false, bool right_side = false);
    static void setComponents(QFormLayout *layout, QWidget *widget, const QString &labelValue, int row = 0, bool boldLabel = false, bool right_side = false);
    /**
      * Podobnie jak wyżej. Zamiast QString labelValue przyjmuje QLabela
      * @see setComponents()
      */
    static void setComponents(QGridLayout *layout, QWidget *widget, QLabel *label, int row, int col, int dy, int dx);
//Dodanie samej QLabel do formatki
    static void setComponents(QGridLayout *layout, const QString &labelValue, int row, int col, bool boldLabel = false);
    static void setComponents(QFormLayout *layout, const QString &labelValue, int row = 0, bool boldLabel = false);
    static void setComponents(QGridLayout *layout, QLabel *label, int row, int col);

  protected:
    FormFieldList m_fields;
    bool m_initialized;

  private slots:
    void validateForm();
    void rejectForm();

  private:
    //std::function<QString (QList<CFormAbstractField*>)> m_validateCallback;
    validateFunc m_validateCallback;
    CGridLayout *m_grid;
};


#endif // FORM_H
