#ifndef CEXPORTCSV_H
#define CEXPORTCSV_H

#include "Global.h"

class CExportCsv
{
    ADD_PTR(QFile, m_file, getFile)
    ADD_FIELD(QString, m_fileName, getFileName, setFileName)
    ADD_FIELD(QString, m_codecName, getCodecName, setCodecName)
    ADD_FIELD(bool, m_isTitle, getIsTitle, setIsTitle)
    ADD_FIELD(bool, m_open, getIsOpen, setIsOpen)

public:

    CExportCsv(const QString &fileName, const QChar &delimeter = QChar(';'),
               const QString &codecName = QString("iso8859-2"),
               const QChar & digitSign = QChar('.'), bool isTitle = true,
               bool textDelimiter = false, bool todos = true);
    ~CExportCsv();

    void setDelimeter(const QChar &delimeter = QChar(';'));
    void setCodec(const QString &codecName = QString("iso8859-2"));
    void setDigitSign(const QChar &digitSign = QChar('.'));
    void setToDos(bool todos = true);
    void setTextDelimiter(bool textDelimiter = true);

    void writeHeader(QStringList data);
    void writeHeader(QVector<QVariant> data);
    void writeLine(QVector<QVariant> data);
    void writeLine(const QString &data);
    void writeLine(QStringList data);

private:

    QTextStream *m_out;
    QChar m_delimiter;
    QChar m_digitSign;
    bool m_textDelimiter;
    bool m_todos;
    QString m_end_of_line;
};

#endif // CEXPORTCSV_H
