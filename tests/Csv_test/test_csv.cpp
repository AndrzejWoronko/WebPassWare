#include <QObject>
#include <QString>
#include <QtTest>
#include <QFile>
#include <QTemporaryDir>

#include "ExportCsv.h"
#include "ImportCsv.h"
#include "CsvCodec.h"

using namespace wpw;

// Pins the bytes the CSV export writes and what the import reads back for
// every encoding offered in the dialogs, so the move away from QTextCodec
// keeps files compatible with the ones users already have.
class Test_Csv : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test_exportBytes_data();
    void test_exportBytes();
    void test_importReadsBack_data();
    void test_importReadsBack();
    void test_importQuotedFields();
    void test_roundTripSpecialCharacters();
    void test_exportVariants();
    void test_dialogChoices();
    void test_importStripsUtf8Bom();

private:
    QString writeFile(const QString &name, const QByteArray &bytes);

    QTemporaryDir m_dir;
};

static const QString kText = QString::fromUtf8("Zażółć gęślą jaźń");
static const QString kShort = QString::fromUtf8("ąśź");

void Test_Csv::initTestCase()
{
    QVERIFY(m_dir.isValid());
}

QString Test_Csv::writeFile(const QString &name, const QByteArray &bytes)
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

void Test_Csv::test_exportBytes_data()
{
    QTest::addColumn<QString>("codec");
    QTest::addColumn<QByteArray>("line");

    // Encoded with Python's codecs, independent of Qt.
    QTest::newRow("ISO-8859-2") << QString("ISO-8859-2") << QByteArray::fromHex("5a61bff3b3e62067eab66cb1206a61bcf13bb1b6bc");
    QTest::newRow("Windows-1250") << QString("Windows-1250") << QByteArray::fromHex("5a61bff3b3e62067ea9c6cb9206a619ff13bb99c9f");
    QTest::newRow("UTF-8") << QString("UTF-8") << kText.toUtf8() + ";" + kShort.toUtf8();
    // The dialogs' name for code page 852 (DOS Latin 2).
    QTest::newRow("DOS/OS2-852") << QString("DOS/OS2-852") << QByteArray::fromHex("5a61bea288862067a9986ca5206a61abe43ba598ab");
}

void Test_Csv::test_exportBytes()
{
    QFETCH(QString, codec);
    QFETCH(QByteArray, line);

    const QString path = m_dir.filePath(QString("export-%1.csv").arg(QString(QTest::currentDataTag()).replace('/', '_')));
    {
        CExportCsv csv(path, QChar(';'), codec, QChar(','));
        QVERIFY(csv.getIsOpen());
        csv.writeLine(QVector<QVariant>() << QVariant(QString("tytul")) << QVariant(QString("haslo")));
        csv.writeLine(QVector<QVariant>() << QVariant(kText) << QVariant(kShort));
        csv.writeLine(QVector<QVariant>() << QVariant(QString("kwota")) << QVariant(12.5));
    }

    QFile file(path);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const QByteArray expected = QByteArray("tytul;haslo\r\n") + line + "\r\n" + "kwota;12,5\r\n";
    QCOMPARE(file.readAll(), expected);
}

void Test_Csv::test_importReadsBack_data()
{
    test_exportBytes_data();
}

void Test_Csv::test_importReadsBack()
{
    QFETCH(QString, codec);
    QFETCH(QByteArray, line);

    const QString path = writeFile(QString("import-%1.csv").arg(QString(QTest::currentDataTag()).replace('/', '_')),
                                   QByteArray("tytul;haslo\r\n") + line + "\r\n");
    QVERIFY(!path.isEmpty());

    CImportCsv csv(path, QChar(';'), codec, QChar(','), true);
    QVERIFY(csv.openFile());
    QCOMPARE(csv.getTitle(), QStringList() << "tytul" << "haslo");
    QCOMPARE(csv.getRowCount(), qint64(1));

    // openFile() rewinds to the start, so the header is read again first.
    QVERIFY(csv.readLine());
    QCOMPARE(csv.getBuff(), QStringList() << "tytul" << "haslo");
    QVERIFY(csv.readLine());
    QCOMPARE(csv.getBuff(), QStringList() << kText << kShort);
    QCOMPARE(csv.getFieldByName("haslo"), kShort);
    QVERIFY(!csv.readLine());
}

void Test_Csv::test_importQuotedFields()
{
    // RFC 4180: quoted fields may hold the delimiter, doubled quotes and line breaks.
    const QString path = writeFile("quoted.csv", QByteArray("a;b\n\"x;y\";z\n\"say \"\"hi\"\"\";\"two\nlines\"\nplain;\n"));
    QVERIFY(!path.isEmpty());

    CImportCsv csv(path, QChar(';'), QString("UTF-8"), QChar(','), false);
    QVERIFY(csv.openFile());
    QCOMPARE(csv.getRowCount(), qint64(4));
    QVERIFY(csv.readLine());
    QCOMPARE(csv.getBuff(), QStringList() << "a" << "b");
    QVERIFY(csv.readLine());
    QCOMPARE(csv.getBuff(), QStringList() << "x;y" << "z");
    QVERIFY(csv.readLine());
    QCOMPARE(csv.getBuff(), QStringList() << "say \"hi\"" << "two\nlines");
    QVERIFY(csv.readLine());
    QCOMPARE(csv.getBuff(), QStringList() << "plain" << "");
    QVERIFY(!csv.readLine());
}

void Test_Csv::test_roundTripSpecialCharacters()
{
    const QString path = m_dir.filePath("roundtrip.csv");
    const QStringList row = QStringList() << "Bank" << "p;a\"ss" << "linia1\nlinia2";
    {
        CExportCsv csv(path, QChar(';'), QString("UTF-8"), QChar(','));
        QVERIFY(csv.getIsOpen());
        csv.writeLine(QVector<QVariant>() << QVariant(row.at(0)) << QVariant(row.at(1)) << QVariant(row.at(2)));
    }
    CImportCsv csv(path, QChar(';'), QString("UTF-8"), QChar(','), false);
    QVERIFY(csv.openFile());
    QVERIFY(csv.readLine());
    QCOMPARE(csv.getBuff(), row);
    QVERIFY(!csv.readLine());
}

void Test_Csv::test_exportVariants()
{
    const QString path = m_dir.filePath("variants.csv");
    {
        CExportCsv csv(path, QChar(','), QString("UTF-8"), QChar('.'), true);
        QVERIFY(csv.getIsOpen());
        csv.setToDos(false);
        csv.writeHeader(QStringList() << "a" << "b");
        csv.writeHeader(QVector<QVariant>() << QVariant(QString("c")) << QVariant(QString("d")));
        csv.writeLine(QStringList() << "x,y" << "z");
        csv.writeLine(QString("raw line"));
        csv.setTextDelimiter(true);
        csv.writeLine(QStringList() << "p" << "q\"r");
        csv.writeLine(QVector<QVariant>() << QVariant(QString("s")) << QVariant(1.5) << QVariant(QString("t")));
        csv.writeHeader(QStringList() << "h1" << "h2");
        csv.setDelimeter(QChar(';'));
        csv.setDigitSign(QChar(','));
        csv.setCodec(QString("UTF-8"));
        csv.writeLine(QVector<QVariant>() << QVariant(QString("u")) << QVariant(2.5));
    }
    QFile file(path);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(QString::fromUtf8(file.readAll()),
             QString("a,b\n"
                     "c,d\n"
                     "\"x,y\",z\n"
                     "raw line\n"
                     "\"p\",\"q\"\"r\"\n"
                     "\"s\",1.5, \"t\"\n"
                     "\"h1\",\"h2\"\n"
                     "\"u\";2,5\n"));

    // Without a title row the header is skipped.
    const QString noTitle = m_dir.filePath("notitle.csv");
    {
        CExportCsv csv(noTitle, QChar(';'), QString("UTF-8"), QChar('.'), false, false, false);
        csv.writeHeader(QStringList() << "a");
        csv.writeLine(QStringList() << "b");
    }
    QFile second(noTitle);
    QVERIFY(second.open(QIODevice::ReadOnly));
    QCOMPARE(second.readAll(), QByteArray("b\n"));
}

void Test_Csv::test_dialogChoices()
{
    // One list for the import and the export dialog.
    QCOMPARE(wpw::csvCodecNames(), QStringList() << "ISO-8859-2" << "Windows-1250" << "UTF-8" << "DOS/OS2-852");
    QCOMPARE(wpw::csvSeparatorNames(), QStringList() << "TAB" << "|" << ";" << ",");
    QCOMPARE(wpw::csvDigitSigns(), QStringList() << "," << ".");
    QCOMPARE(wpw::csvSeparator("TAB"), QChar('\t'));
    QCOMPARE(wpw::csvSeparator(";"), QChar(';'));
    QVERIFY(wpw::csvSeparator(QString()).isNull());
}

void Test_Csv::test_importStripsUtf8Bom()
{
    // Spreadsheets often save UTF-8 with a byte order mark.
    const QString path = writeFile("bom.csv", QByteArray("\xEF\xBB\xBFtytul;haslo\nBank;sekret\n"));
    QVERIFY(!path.isEmpty());

    CImportCsv csv(path, QChar(';'), QString("UTF-8"), QChar(','), true);
    QVERIFY(csv.openFile());
    QCOMPARE(csv.getTitle(), QStringList() << "tytul" << "haslo");
    QCOMPARE(csv.getFieldByName("tytul"), QString());
    QVERIFY(csv.readLine());
    QVERIFY(csv.readLine());
    QCOMPARE(csv.getFieldByName("tytul"), QString("Bank"));
}

QTEST_MAIN(Test_Csv)

#include "test_csv.moc"
