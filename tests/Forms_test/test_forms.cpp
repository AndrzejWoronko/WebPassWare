#include <QObject>
#include <QString>
#include <QtTest>
#include <QSettings>
#include <QTemporaryDir>

#include "FormCheckField.h"
#include "FormNumberField.h"
#include "FormSimpleChoiceField.h"
#include "FormTextField.h"
#include "AbstractFormDialogController.h"

using namespace wpw;

// isDirty() compares the start value with the widget value as QVariants.
// Dialogs restore both from QSettings, which hands strings back, so these
// tests pin that an untouched field restored this way is not dirty.
class Test_Forms : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test_untouchedFieldsAreClean();
    void test_restoredFromSettingsAreClean();
    void test_editedFieldsAreDirty();
    void test_emptyRequiredFields();

private:
    QVariant roundTrip(const QString &key, const QVariant &value);

    QTemporaryDir m_dir;
};

void Test_Forms::initTestCase()
{
    QVERIFY(m_dir.isValid());
}

// QSettings caches values per process, so the file is written by hand, with
// values quoted the way QSettings stores strings: this
// is what the dialog reads back after a restart.
QVariant Test_Forms::roundTrip(const QString &key, const QVariant &value)
{
    const QString path = m_dir.filePath(key + ".ini");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return QVariant();
    }
    file.write(QString("[General]\n%1=\"%2\"\n").arg(key, value.toString()).toUtf8());
    file.close();
    QSettings settings(path, QSettings::IniFormat);
    return settings.value(key);
}

void Test_Forms::test_untouchedFieldsAreClean()
{
    CFormTextField text("m_title", "Tytuł", "Email");
    CFormCheckField check("m_isTitle", "Czy jest tytuł ?", true);
    CFormNumberIntField intNumber("m_count", "Ilość", 3);
    CFormNumberField number("m_fromLine", "Od linii", 0);
    CFormSimpleChoiceField choice("m_fieldsSeparator", "Separator", 0, QStringList() << ";" << ",");

    QVERIFY(!text.isDirty());
    QVERIFY(!check.isDirty());
    QVERIFY(!intNumber.isDirty());
    QVERIFY(!number.isDirty());
    // The start value 0 is not the displayed text ";"; isDirty() only skips -1.
    QVERIFY(choice.isDirty());
}

void Test_Forms::test_restoredFromSettingsAreClean()
{
    CFormCheckField check("m_isTitle", "Czy jest tytuł ?", false);
    const QVariant checkValue = roundTrip(check.getVariableName(), true);
    QCOMPARE(checkValue.userType(), int(QMetaType::QString));
    check.setValue(checkValue);
    check.setStartValue(checkValue);
    QVERIFY(!check.isDirty());

    CFormNumberField number("m_fromLine", "Od linii", 0);
    const QVariant numberValue = roundTrip(number.getVariableName(), 2);
    number.setValue(numberValue);
    number.setStartValue(numberValue);
    QVERIFY(!number.isDirty());

    CFormNumberIntField intNumber("m_count", "Ilość", 0);
    const QVariant intValue = roundTrip(intNumber.getVariableName(), 7);
    intNumber.setValue(intValue);
    intNumber.setStartValue(intValue);
    QVERIFY(!intNumber.isDirty());

    CFormSimpleChoiceField choice("m_fieldsSeparator", "Separator", 0, QStringList() << ";" << ",");
    const QVariant choiceValue = roundTrip(choice.getVariableName(), QString(","));
    choice.setValue(choiceValue);
    choice.setStartValue(choiceValue);
    QVERIFY(!choice.isDirty());
}

void Test_Forms::test_editedFieldsAreDirty()
{
    CFormTextField text("m_title", "Tytuł", "Email");
    text.setText("Poczta");
    QVERIFY(text.isDirty());

    CFormCheckField check("m_isTitle", "Czy jest tytuł ?", true);
    check.setChecked(false);
    QVERIFY(check.isDirty());

    CFormNumberField number("m_fromLine", "Od linii", 0);
    number.QDoubleSpinBox::setValue(4);
    QVERIFY(number.isDirty());
}

void Test_Forms::test_emptyRequiredFields()
{
    CFormTextField title("m_title", "Tytuł", "Email");
    CFormTextField user("m_user", "Użytkownik", "");
    CFormTextField pass("m_pass", "Hasło", "");
    FormFieldHashList fields;
    fields.insert("m_title", &title);
    fields.insert("m_user", &user);
    fields.insert("m_pass", &pass);

    // In the order of the names; fields that are not in the form are skipped.
    QCOMPARE(CAbstractFormDialogController::emptyRequiredFields(fields, {"m_title", "m_user", "m_pass", "m_missing"}),
             QString::fromUtf8("Puste pole: Użytkownik:.\nPuste pole: Hasło:.\n"));
    QCOMPARE(CAbstractFormDialogController::emptyRequiredFields(fields, {"m_title"}), QString());
}

QTEST_MAIN(Test_Forms)

#include "test_forms.moc"
