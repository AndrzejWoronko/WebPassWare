#ifndef LOGGER_H
#define LOGGER_H

#include "Global.h"
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QTextStream>

//klasa log`a dziennego lub miesiecznego

class CLogger : public QTextStream
{
private:
    QFile m_file;
    QString m_fileName;
    bool m_month;
    bool m_screen;
    void mkDirIfNotExist();  //tworzenie katalogu jeżeli nie istnieje
    void setFile(void);
public:    
    CLogger(bool miesiac=true, bool console = true);
    CLogger(const QString &fname, bool console = true);
    CLogger(const QString &fname, bool miesiac, bool console = true);
    ~CLogger();
    static const QString PATH;
    QString getLogFilePath();
    bool getScreen();
};

class CAppLogger: public CLogger, public Singleton<CAppLogger>
{
public:
    CAppLogger();
    ~CAppLogger() {}
    //static CAppLogger &getInstance();
};

#define LOG_APP Qt::endl(CAppLogger::getInstance()) << LOG_DATE_LINE

#define LOG_DIR      "logi"
#define SQL_LOG_FILE "sql"

class CSqlLogger: public CLogger, public Singleton<CSqlLogger>
{
public:
    CSqlLogger();
    ~CSqlLogger() {}
    //static CSqlLogger &getInstance();
};

#define LOG_SQL Qt::endl(CSqlLogger::getInstance()) << LOG_DATE_LINE

#endif // LOGGER_H
