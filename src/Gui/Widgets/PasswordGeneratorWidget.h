#ifndef CPASSWORDGENERATORWIDGET_H
#define CPASSWORDGENERATORWIDGET_H

#include "Global.h"
#include <QWidget>
#include "PasswordGenerator.h"
#include "Form.h"
#include "ToolButton.h"
#include <QSlider>
#include <QGroupBox>
#include "Layouts.h"

namespace wpw {

inline constexpr char GENERATOR_SETTING_ENTRY_LENGTH[] = "Generator/Length";
inline constexpr char GENERATOR_SETTING_ENTRY_UPPER_CASE[] = "Generator/UpperCase";
inline constexpr char GENERATOR_SETTING_ENTRY_LOWER_CASE[] = "Generator/LowerCase";
inline constexpr char GENERATOR_SETTING_ENTRY_NUMBERS[] = "Generator/Numbers";
inline constexpr char GENERATOR_SETTING_ENTRY_SPECIAL_CHARS[] = "Generator/SpecialChars";
inline constexpr char GENERATOR_SETTING_ENTRY_EXCLUDE_ALIKE[] = "Generator/ExcludeAlike";
inline constexpr char GENERATOR_SETTING_ENTRY_ENSURE_EVERY[] = "Generator/EnsureEvery";

class CPasswordGeneratorWidget : public QWidget
{
   Q_OBJECT

   ADD_PTR_PROPERTY(CFormTextField, m_password, getPassword)
   ADD_PTR_PROPERTY(CFormNumberIntField, m_length, getLength)
   ADD_PTR_PROPERTY(QSlider, m_sliderLength, getSliderLenght)

   ADD_PTR_PROPERTY(CToolButton, m_checkButtonUpper, getCheckButtonUpper)
   ADD_PTR_PROPERTY(CToolButton, m_checkButtonLower, getCheckButtonLower)
   ADD_PTR_PROPERTY(CToolButton, m_checkButtonNumbers, getCheckButtonNumbers)
   ADD_PTR_PROPERTY(CToolButton, m_checkButtonSpecialChars, getCheckButtonSpecialChars)

   ADD_PTR_PROPERTY(CFormCheckField, m_checkBoxEnsureEvery, getCheckBoxEnsureEvery)
   ADD_PTR_PROPERTY(CFormCheckField, m_checkBoxExcludeAlike, getCheckBoxExcludeAlike)

   ADD_PTR_PROPERTY(CButton, m_acceptButton, getAcceptButton)
   ADD_PTR_PROPERTY(CButton, m_generateButton, getGenerateButton)

public:
    explicit CPasswordGeneratorWidget(QWidget* parent = nullptr);
    ~CPasswordGeneratorWidget();
    void loadSettings();
    void reset();
    void regeneratePassword();

Q_SIGNALS:
    void newPassword(const QString& password);

private Q_SLOTS:
    void updateApplyEnabled(const QString& password);

    void emitNewPassword();
    void saveSettings();
    void sliderMoved();
    void spinBoxChanged();

    void updateGenerator();

private:

    void initWidget();

    bool m_updatingSpinBox;

    CPasswordGenerator::CharClasses charClasses();
    CPasswordGenerator::GeneratorFlags generatorFlags();

    CPasswordGenerator *m_generator;
};

} // namespace wpw

#endif // CPASSWORDGENERATORWIDGET_H
