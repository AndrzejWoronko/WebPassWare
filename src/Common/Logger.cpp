#include "Logger.h"

const QString CLogger::PATH = QDir::homePath() + QDir::separator() + QString(LOG_DIR);
//const QString CLogger::PATH = QString(PROGRAM_DIR) + QDir::separator() + QString(LOG_DIR);

CLogger::CLogger(bool month, bool console)
{
    QString fileName;
    QDateTime dzis = QDateTime::currentDateTime();
    mkDirIfNotExist();

    m_month = month;
    m_screen = console;
    if (m_month)
        fileName = "log_" + dzis.toString(MONTH_FORMAT) + ".log";
    else
        fileName = "log_" + dzis.toString(DATE_FORMAT) + ".log";

    m_fileName = CLogger::PATH + QDir::separator() + fileName;
    this->setFile();
}

CLogger::CLogger(const QString &fname, bool console)
{
    QString fileName;

    mkDirIfNotExist();
    m_screen = console;
    fileName = fname + ".log";
    m_fileName = CLogger::PATH + QDir::separator() + fileName;
    this->setFile();
}

CLogger::CLogger(const QString &fname, bool month, bool console)
{
    QString fileName;
    QDateTime dzis = QDateTime::currentDateTime();

    mkDirIfNotExist();
    m_month = month;
    m_screen = console;
    if (m_month)
        fileName = fname + dzis.toString("yyyy-MM") + ".log";
    else
        fileName = fname + dzis.toString("yyyy-MM-dd") + ".log";
    m_fileName = CLogger::PATH + QDir::separator() + fileName;
     this->setFile();
}

CLogger::~CLogger()
{
    if (m_file.isOpen())
     m_file.close();
}

void CLogger::setFile(void)
{
    m_file.setFileName(m_fileName);
    m_file.open(QFile::WriteOnly | QFile::Append);
    this->setDevice(&m_file);
}

void CLogger::mkDirIfNotExist()
{
    QDir dir(CLogger::PATH);

    if(!dir.exists(CLogger::PATH))
        dir.mkdir(CLogger::PATH);
}

QString CLogger::getLogFilePath()
{
    return this->m_fileName;
}

bool CLogger::getScreen()
{
    return this->m_screen;
}

CAppLogger::CAppLogger() : CLogger(true, true)
{

}

//CAppLogger &CAppLogger::getInstance()
//{
//    static CAppLogger instance;
//    return instance;
//}

CSqlLogger::CSqlLogger() : CLogger(SQL_LOG_FILE, true, true)
{

}

//CSqlLogger &CSqlLogger::getInstance()
//{
//    static CSqlLogger instance;
//    return instance;
//}
