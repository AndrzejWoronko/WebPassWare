#include "ExportCsv.h"

CExportCsv::CExportCsv(const QString &fileName, const QChar &delimeter, const QString &codecName,
                       const QChar & digitSign, bool isTitle, bool textDelimiter, bool todos)
{
    m_fileName  = fileName;

    m_file = new QFile(m_fileName);
    m_open = m_file->open(QIODevice::WriteOnly | QIODevice::Text);
    m_out = new QTextStream(m_file);
    m_codecName = codecName;
    m_delimiter = delimeter;
    m_digitSign = digitSign;
    m_textDelimiter = textDelimiter;
    m_out->setCodec(m_codecName.toLatin1());
    m_todos = todos;
    m_end_of_line = m_todos ? QString("\r\n") : QString("\n");
    m_isTitle = isTitle;
}

CExportCsv::~CExportCsv()
{
    safe_delete(m_out)
    if (m_file)
    {
        if (m_file->isOpen())
            m_file->close();
        safe_delete(m_file)
    }
}

void CExportCsv::setDelimeter(const QChar &delimeter)
{
    m_delimiter = delimeter;
}

void CExportCsv::setTextDelimiter(bool textDelimiter)
{
    m_textDelimiter = textDelimiter;
}

void CExportCsv::setCodec( const QString &codecName)
{
    m_codecName = codecName;
    m_out->setCodec(m_codecName.toLatin1());
}

void CExportCsv::setDigitSign( const QChar &digitSign)
{
    m_digitSign = digitSign;
}

void CExportCsv::setToDos(bool todos)
{
    m_todos = todos;
    m_end_of_line = m_todos ? QString("\r\n") : QString("\n");
}

void CExportCsv::writeHeader(QVector<QVariant> data)
{
    if (m_isTitle)
    {
        for(int i = 0; i < data.length() - 1; i++)
        {
            if (m_textDelimiter)
                *m_out << "\"" << data.at(i).toString().replace("\"","'") << "\"" << m_delimiter;
            else
                *m_out << data.at(i).toString() << m_delimiter;
        }
        if (m_textDelimiter)
            *m_out << "\"" << data.last().toString().replace("\"","'") << "\"" << m_end_of_line;
        else
            *m_out << data.last().toString() << m_end_of_line;
    }
}

void CExportCsv::writeHeader(QStringList data)
{
    if (m_isTitle)
    {
        for(int i = 0; i < data.length() - 1; i++)
        {
            if (m_textDelimiter)
                *m_out << "\"" << data[i].replace("\"","'") << "\"" << m_delimiter;
            else
                *m_out << data.at(i) << m_delimiter;
        }
        if (m_textDelimiter)
            *m_out << "\"" << data.last().replace("\"","'") << "\"" << m_end_of_line;
        else
            *m_out << data.last() << m_end_of_line;
    }
}

void CExportCsv::writeLine(QVector<QVariant> data)
{
    for(int i = 0; i < data.length() - 1; i++)
    {
        if (data.at(i).type() == QVariant::Double)
        {
            QString s_value = data.at(i).toString().replace(".", m_digitSign);
            *m_out << s_value << m_delimiter << " ";
        }
        else if (data.at(i).type() == QVariant::String && m_textDelimiter)
        {
            *m_out  << "\"" << data.at(i).toString().replace("\"","'")  << "\"" << m_delimiter;
        }
        else
        {
            *m_out << data.at(i).toString() << m_delimiter;
        }
    }

    if (data.last().type() == QVariant::Double)
    {
        QString s_value = data.last().toString().replace(".", m_digitSign);
        *m_out << s_value << m_end_of_line;
    }
    else if (data.last().type() == QVariant::String && m_textDelimiter)
    {
        *m_out  << "\"" << data.last().toString().replace("\"","'")  << "\"" << m_end_of_line;
    }
    else
    {
        *m_out << data.last().toString() << m_end_of_line;
    }
}

void CExportCsv::writeLine(const QString &data)
{
    *m_out << data << m_end_of_line;
}

void CExportCsv::writeLine(QStringList data)
{
    for(int i = 0; i < data.length() - 1; i++)
    {
        if (m_textDelimiter)
            *m_out << "\"" << data[i].replace("\"","'") << "\"" << m_delimiter;
        else
            *m_out << data.at(i) << m_delimiter;
    }
    if (m_textDelimiter)
        *m_out << "\"" << data.last().replace("\"","'") << "\"" << m_end_of_line;
    else
        *m_out << data.last() << m_end_of_line;
}
