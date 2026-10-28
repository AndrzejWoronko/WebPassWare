#include "ImportCsv.h"

#include <QStringDecoder>

#include "CsvCodec.h"
#include "CsvFormat.h"

namespace wpw {

// QTextStream in Qt 6 only knows the Unicode encodings and Latin-1, so the
// file is decoded as a whole (ICU covers ISO-8859-2, Windows-1250 and CP852).
// An unknown name falls back to UTF-8, as the Qt 5 stream kept its default codec.
static QString decodeCsv(const QByteArray &bytes, const QString &codecName)
{
    QStringDecoder decoder(wpw::csvCodecName(codecName).constData());
    if (!decoder.isValid())
    {
        decoder = QStringDecoder(QStringConverter::Utf8);
    }
    return decoder.decode(bytes);
}

CImportCsv::CImportCsv(const QString &fileName, const QChar &delimeter, const QString &codecName, const QChar & digitSign, bool isTitle)
{
    m_fileName  = fileName;
    m_file = new QFile(m_fileName);
    m_in = new QTextStream(&m_content, QIODevice::ReadOnly);
    m_codecName = codecName;
    m_delimiter = delimeter;
    m_digitSign = digitSign;
    m_rowCount = 0;
    m_isTitle = isTitle;
}

CImportCsv::~CImportCsv()
{
    safe_delete(m_in);
    if (m_file)
        {
            if (m_file->isOpen())
                m_file->close();
            safe_delete(m_file);
        }
}

void CImportCsv::setDelimeter(const QChar &delimeter)
{
    m_delimiter = delimeter;
}

void CImportCsv::setCodec( const QString &codecName)
{
    m_codecName = codecName;
}

void CImportCsv::setDigitSign( const QChar &digitSign)
{
    m_digitSign = digitSign;
}

void CImportCsv::readHeader(void)
{
    readRecord(&m_title);
}

bool CImportCsv::readRecord(QStringList *fields)
{
    QString record = m_in->readLine();
    if (record.isNull())
    {
        return false;
    }
    // A quoted field with a line break continues in the next lines.
    while (!wpw::Csv::splitRecord(record, m_delimiter, fields))
    {
        const QString next = m_in->readLine();
        if (next.isNull())
        {
            break;
        }
        record += QChar(u'\n') + next;
    }
    return true;
}

void CImportCsv::headerToString(void)
{
    for (auto s : std::as_const(m_title))
    {
        DEBUG_WITH_LINE << s << " ";
    }
    DEBUG_WITH_LINE << Qt::endl;
}

bool CImportCsv::readLine(void)
{
    return readRecord(&m_buff);
}

bool CImportCsv::openFile()
{
    //QString line;

    if (!m_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        DEBUG_WITH_LINE << "Can`t open file: " << m_fileName;
        return false;
    }
    m_content = decodeCsv(m_file->readAll(), m_codecName);
    m_in->setString(&m_content, QIODevice::ReadOnly);
    //m_in = new QTextStream(m_file);
    //m_in->setCodec("UTF-8");
    if (m_isTitle)
    {
        readHeader();
        headerToString();
    }
    m_rowCount = 0;
    QStringList record;
    do
    {
        readRecord(&record);
        m_rowCount++;
    } while (!m_in->atEnd());
    m_in->seek(0);
    return true;
}

void CImportCsv::closeFile()
{
    if (m_file && m_file->isOpen())
        m_file->close();
}

int CImportCsv::getFieldNrByName(const QString &fieldName)
{
    if (m_title.contains(fieldName))
        return m_title.indexOf(fieldName);
    else
        return -1;
}

QString CImportCsv::getFieldByName(const QString &fieldName)
{
    int nr_field = getFieldNrByName(fieldName);

    if  ((nr_field > -1) && (nr_field < m_buff.size()))
        return  m_buff.at(nr_field);
    else
        return QString();
}

QString CImportCsv::getFieldByNr(int fieldNr)
{
    if  ((fieldNr > -1) && (fieldNr < m_buff.size()))
        return  m_buff.at(fieldNr);
    else
        return QString();
}

QStringList CImportCsv::getBuff()
{
    return m_buff;
}

QStringList CImportCsv::getTitle()
{
    return m_title;
}

} // namespace wpw
