#include "ImportCsv.h"

CImportCsv::CImportCsv(const QString &fileName, const QChar &delimeter, const QString &codecName, const QChar & digitSign, bool isTitle)
{
    m_fileName  = fileName;
    m_file = new QFile(m_fileName);
    m_in = new QTextStream(m_file);
    m_codecName = codecName;
    m_delimiter = delimeter;
    m_digitSign = digitSign;
    m_in->setCodec(m_codecName.toLatin1());
    m_rowCount = 0;
    m_isTitle = isTitle;
}

CImportCsv::~CImportCsv()
{
    safe_delete(m_in)
    if (m_file)
        {
            if (m_file->isOpen())
                m_file->close();
            safe_delete(m_file)
        }
}

void CImportCsv::setDelimeter(const QChar &delimeter)
{
    m_delimiter = delimeter;
}

void CImportCsv::setCodec( const QString &codecName)
{
    m_codecName = codecName;
    m_in->setCodec(m_codecName.toLatin1());
}

void CImportCsv::setDigitSign( const QChar &digitSign)
{
    m_digitSign = digitSign;
}

void CImportCsv::readHeader(void)
{
    QString line;
    line = m_in->readLine();
    m_title = line.split(m_delimiter);
}

void CImportCsv::headerToString(void)
{
    Q_FOREACH(auto s, m_title)
    {
        DEBUG_WITH_LINE << s << " ";
    }
    DEBUG_WITH_LINE << Qt::endl;
}

bool CImportCsv::readLine(void)
{
    QString line;

    line = m_in->readLine();
    if (line.isNull())
        return false;
    else
        m_buff = line.split(m_delimiter);
    return true;
}

bool CImportCsv::openFile()
{
    //QString line;

    if (!m_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        DEBUG_WITH_LINE << "Can`t open file: " << m_fileName;
        return false;
    }
    //m_in = new QTextStream(m_file);
    //m_in->setCodec("UTF-8");
    if (m_isTitle)
    {
        readHeader();
        headerToString();
    }
    m_rowCount = 0;
    do
    {
        m_in->readLine();
        m_rowCount++;

        //} while (!line.isNull());
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
