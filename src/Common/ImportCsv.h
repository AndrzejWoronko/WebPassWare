#ifndef CIMPORTCSV_H
#define CIMPORTCSV_H

#include "Global.h"

class CImportCsv
{
    ADD_FIELD(qint64, m_rowCount, getRowCount, setRowCount)
    ADD_FIELD(bool, m_isTitle, getIsTitle, setIsTitle)
    ADD_PTR(QFile, m_file, getFile)
    ADD_FIELD(QString, m_fileName, getFileName, setFileName)
    ADD_FIELD(QString, m_codecName, getCodecName, setCodecName)

public:

    CImportCsv(const QString &fileName, const QChar &delimeter = QChar(';'),
               const QString &codecName = QString("iso8859-2"),
               const QChar & digitSign = QChar('.'), bool isTitle = true);
    ~CImportCsv();
    bool openFile();
    void closeFile();
    void setDelimeter(const QChar &delimeter = QChar(';'));
    void setCodec(const QString &codecName = QString("iso8859-2"));
    void setDigitSign( const QChar &digitSign = QChar('.'));

    bool readLine(void);

    QString getFieldByName(const QString &fieldName);
    QString getFieldByNr(int fieldNr);
    QStringList getBuff();
    QStringList getTitle();
    void readHeader(void);

private:

    int getFieldNrByName(const QString &fieldName);
    void headerToString(void);

    QTextStream *m_in;
    QChar m_delimiter;
    QChar m_digitSign;
    QStringList m_title;
    QStringList m_buff;
};

#endif // CIMPORTCSV_H
