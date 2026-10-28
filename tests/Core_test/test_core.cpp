#include <QObject>
#include <QString>
#include <QtTest>
#include <QFile>
#include <QTemporaryDir>

#include "CsvModel.h"
#include "ImportCsv.h"
#include "SqlField.h"
#include "Tools.h"

using namespace wpw;

// Helpers of wpw_core without a database: number formatting and names
// (Tools), column types of the list views (CSqlField), the CSV preview
// model of the import dialog (CCsvModel).
class Test_Core : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test_roundPrec();
    void test_niceNumbers();
    void test_names();
    void test_tagValue();
    void test_memoryUsage();
    void test_sqlFieldTypes_data();
    void test_sqlFieldTypes();
    void test_sqlFieldKinds();
    void test_csvModelWithTitle();
    void test_csvModelWithoutTitle();

private:
    QString writeFile(const QString &name, const QByteArray &bytes);

    QTemporaryDir m_dir;
};

void Test_Core::initTestCase()
{
    QVERIFY(m_dir.isValid());
}

QString Test_Core::writeFile(const QString &name, const QByteArray &bytes)
{
    const QString path = m_dir.filePath(name);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        return QString();
    }
    file.write(bytes);
    return path;
}

void Test_Core::test_roundPrec()
{
    QCOMPARE(Tools::round_prec(3.14159, 2), QString("3.14"));
    QCOMPARE(Tools::round_prec(2.5, 0), QString("3"));
    QCOMPARE(Tools::round_prec(7.0, 3), QString("7.000"));
    QCOMPARE(Tools::round_prec_double(3.14159, 3), 3.142);
}

void Test_Core::test_niceNumbers()
{
    QCOMPARE(Tools::double2NiceString(12.5, 2), QString("12,50"));
    QCOMPARE(Tools::double2NiceString(1234.5, 2), QString("1 234,50"));
    QCOMPARE(Tools::double2NiceString2Lines(12.5, 2), QString("12,50"));
    QCOMPARE(Tools::double2NiceString2Lines(1234.5, 2), QString("1 234,\n50"));
}

void Test_Core::test_names()
{
    QCOMPARE(Tools::snakeCase("PassEntry"), QString("pass_entry"));
    QCOMPARE(Tools::snakeCase("HTTPServer"), QString("http_server"));
    QCOMPARE(Tools::snakeCase("ABC"), QString("abc"));
    QCOMPARE(Tools::tableize("PassGroup"), QString("pass_group"));
    QCOMPARE(Tools::tableize("Car"), QString("car"));
}

void Test_Core::test_tagValue()
{
    QCOMPARE(Tools::getTagValue("<root><ver>1.2</ver></root>", "ver"), QString("1.2"));
    QCOMPARE(Tools::getTagValue("<root></root>", "ver"), QString());
}

void Test_Core::test_memoryUsage()
{
    QVERIFY(Tools::getMemoryUsage() > 0);
    QVERIFY(!Tools::getMemoryUsageToString().isEmpty());
    Tools::sleep(1);
    Tools::wait(1);
}

void Test_Core::test_sqlFieldTypes_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("type");
    QTest::addColumn<QString>("back");

    QTest::newRow("string") << "string" << int(QMetaType::QString) << "string";
    QTest::newRow("long") << "long" << int(QMetaType::Int) << "long";
    QTest::newRow("int") << "int" << int(QMetaType::Int) << "long";
    QTest::newRow("double") << "double" << int(QMetaType::Double) << "double";
    QTest::newRow("date") << "date" << int(QMetaType::QDate) << "data";
    QTest::newRow("data") << "data" << int(QMetaType::QDate) << "data";
    QTest::newRow("ilosc") << "ilosc" << int(QMetaType::Double) << "double";
    QTest::newRow("wartosc") << "wartosc" << int(QMetaType::Double) << "double";
    QTest::newRow("datetime") << "datetime" << int(QMetaType::QDateTime) << "datetime";
    QTest::newRow("time") << "time" << int(QMetaType::QTime) << "time";
    QTest::newRow("czas") << "czas" << int(QMetaType::QDateTime) << "datetime";
    QTest::newRow("quantity") << "quantity" << int(QMetaType::Double) << "double";
    QTest::newRow("amount") << "amount" << int(QMetaType::Double) << "double";
    QTest::newRow("bool") << "bool" << int(QMetaType::Bool) << "bool";
    QTest::newRow("check") << "check" << int(QMetaType::Bool) << "bool";
    QTest::newRow("Value") << "Value" << int(QMetaType::Double) << "double";
    QTest::newRow("Price") << "Price" << int(QMetaType::Double) << "double";
    QTest::newRow("Amount") << "Amount" << int(QMetaType::Double) << "double";
    QTest::newRow("Indicator") << "Indicator" << int(QMetaType::Double) << "double";
    QTest::newRow("Percent") << "Percent" << int(QMetaType::Double) << "double";
    QTest::newRow("Varchar255") << "Varchar255" << int(QMetaType::QString) << "string";
    QTest::newRow("Color") << "Color" << int(QMetaType::QString) << "string";
    QTest::newRow("Timestamp") << "Timestamp" << int(QMetaType::QDateTime) << "datetime";
    QTest::newRow("unknown") << "xyz" << int(QMetaType::QString) << "string";
}

void Test_Core::test_sqlFieldTypes()
{
    QFETCH(QString, name);
    QFETCH(int, type);
    QFETCH(QString, back);

    QCOMPARE(int(CSqlField::recognizeString2Type(name)), type);
    QCOMPARE(CSqlField::recognizeType2String(CSqlField::recognizeString2Type(name)), back);
}

void Test_Core::test_sqlFieldKinds()
{
    CSqlField id("long", "Id", "Id", 0);
    QVERIFY(id.isNumericField());
    QVERIFY(!id.isDateField());
    QCOMPARE(id.getName(), QString("Id"));
    QCOMPARE(id.getPos(), 0);

    CSqlField price("Price", "Cena", "Cena", 1);
    QVERIFY(price.isNumericField());

    CSqlField title("string", "Tytuł", "Tytuł", 2);
    QVERIFY(!title.isNumericField());
    QVERIFY(!title.isDateField());

    CSqlField when("datetime", "Kiedy", "Kiedy", 3);
    QVERIFY(when.isDateField());
    QVERIFY(CSqlField::positionLessThan(&id, &title));
    QCOMPARE(CSqlField::recognizeType2String(QMetaType::QByteArray), QString("string"));
}

void Test_Core::test_csvModelWithTitle()
{
    const QString path = writeFile("model.csv", QByteArray("tytul;uzytkownik;haslo\nBank; jan ;sekret\nPoczta;ewa\n"));
    QVERIFY(!path.isEmpty());

    CCsvModel model(path, QChar(';'), QString("UTF-8"), QChar(','), true);
    model.refresh();
    QCOMPARE(model.getHeaders(), QStringList() << "tytul" << "uzytkownik" << "haslo");
    QCOMPARE(model.rowCount(QModelIndex()), 2);
    QCOMPARE(model.columnCount(QModelIndex()), 3);
    QCOMPARE(model.getTableSize(), 2);
    // Values are trimmed.
    QCOMPARE(model.data(model.index(0, 1)).toString(), QString("jan"));
    // A short row gives empty cells.
    QCOMPARE(model.data(model.index(1, 2)).toString(), QString());
    QCOMPARE(model.headerData(2, Qt::Horizontal).toString(), QString("haslo"));
    QCOMPARE(model.getColumnForImport(), QStringList() << "BRAK" << "tytul" << "uzytkownik" << "haslo");
    QCOMPARE(model.getValueForImport(1, "tytul").toString(), QString("Poczta"));
    QVERIFY(!model.getValueForImport(0, "brak").isValid());

    model.setFromLine(1);
    QCOMPARE(model.rowCount(QModelIndex()), 1);
    QCOMPARE(model.data(model.index(0, 0)).toString(), QString("Poczta"));
}

void Test_Core::test_csvModelWithoutTitle()
{
    const QString path = writeFile("plain.csv", QByteArray("a;b\nc;d;e\n"));
    QVERIFY(!path.isEmpty());

    QSharedPointer<CImportCsv> import(new CImportCsv(path, QChar(';'), QString("UTF-8"), QChar(','), false));
    CCsvModel model(import);
    model.setIsTitle(false);
    model.refresh();
    QCOMPARE(model.rowCount(QModelIndex()), 2);
    QCOMPARE(model.columnCount(QModelIndex()), 3);
    QCOMPARE(model.getColumnForImport(), QStringList() << "BRAK" << "1" << "2" << "3");
    QCOMPARE(model.getValueForImport(1, "3").toString(), QString("e"));
    QVERIFY(!model.data(model.index(0, 0), Qt::EditRole).isValid());
}

QTEST_GUILESS_MAIN(Test_Core)

#include "test_core.moc"
