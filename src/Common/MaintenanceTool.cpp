#include "MaintenanceTool.h"
#include "ApplicationSettings.h"
#include "Application.h"
//#include "MessageBox.h"

CMaintenanceTool::CMaintenanceTool(QObject *parent) : QObject(parent), m_state(CMaintenanceTool::NotRunning), m_hasUpdate(false)
{
    connect(&m_process, &QProcess::started, this, &CMaintenanceTool::processStarted);
    connect(&m_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &CMaintenanceTool::processFinished);
    connect(&m_process, qOverload<QProcess::ProcessError>(&QProcess::errorOccurred), this, &CMaintenanceTool::processError);
}

void CMaintenanceTool::checkUpdate()
{
    startMaintenanceTool(CMaintenanceTool::CheckUpdate);
}

void CMaintenanceTool::startUpdate()
{
    startMaintenanceTool(CMaintenanceTool::Updater);
}

void CMaintenanceTool::startMaintenanceTool(StartMode mode)
{
    QString toolName;
    QString path;
#if defined(Q_OS_WIN)
    toolName = QString("maintenancetool.exe");
#elif defined(Q_OS_MAC)
    toolName = QString("../../../maintenancetool.app/Contents/MacOS/maintenancetool");
#else
    toolName = QString("maintenancetool");
#endif


    path = QDir(QDir::currentPath()).absoluteFilePath(toolName);
    DEBUG_WITH_LINE << "Installer program: " << path;

    QStringList args;

    args.clear();
    if(mode == CMaintenanceTool::CheckUpdate)
    {
        if(m_process.state() == QProcess::NotRunning)
        {
            setUpdateDetails(QString());
            setHasUpdate(false);

            args.append("check-updates");

            m_process.start(path, args);

            m_process.waitForStarted();

            setState(CMaintenanceTool::Running);

            m_process.waitForFinished();
        }
        else
        {
            DEBUG_WITH_LINE << "Already started.";
        }
    }    
    else if(mode == CMaintenanceTool::Updater)
    {
        args.append("--start-updater");
        QProcess::startDetached(path, args);
        APPI->closeAllWindows(); // Close the application
    }
}

CMaintenanceTool::ProcessState CMaintenanceTool::state() const
{
    return m_state;
}

void CMaintenanceTool::setState(ProcessState state)
{
    if(m_state == state)  return;
    m_state = state;
    emit stateChanged(m_state);
}

bool CMaintenanceTool::hasUpdate() const
{
    return m_hasUpdate;
}

void CMaintenanceTool::setHasUpdate(bool hasUpdate)
{
    if(m_hasUpdate == hasUpdate)  return;
    m_hasUpdate = hasUpdate;
    emit hasUpdateChanged(m_hasUpdate);
}

QString CMaintenanceTool::updateDetails() const
{
    return m_updateDetails;
}

void CMaintenanceTool::setUpdateDetails(const QString &updateDetails)
{
    if(m_updateDetails == updateDetails) return;
    m_updateDetails = updateDetails;
    emit updateDetailsChanged(m_updateDetails);
}

void CMaintenanceTool::processStarted()
{
    setState(CMaintenanceTool::Running);
}

void CMaintenanceTool::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    DEBUG_WITH_LINE << "exitCode=" << exitCode << ", exitStatus=" << exitStatus;

    if(exitCode == 0)
    {
        QByteArray stdOut = m_process.readAllStandardOutput();
        QString stdOutStr = QString::fromLocal8Bit(stdOut);
        DEBUG_WITH_LINE << "out>" << stdOutStr;

        QString xmlStr;
        QStringList lines;
        bool enabled = false;

        lines = stdOutStr.split("\n");

        Q_FOREACH(auto line, lines)
        {
            if(line.startsWith("<updates>"))
            {
                enabled = true;
                xmlStr.append(line);
            }
            else if(line.endsWith("</updates>"))
            {
                xmlStr.append(line);
                break;
            }
            else if(enabled)
            {
                xmlStr.append(line);
            }
            else
            {
                if (line.contains("no updates available"))
                    break;
            }
        }
        if (xmlStr.length() > 0)
           {
              DEBUG_WITH_LINE << "Updates: " << xmlStr;
              setUpdateDetails(xmlStr);
              setHasUpdate(true);
              //CMessageBox::OkDialogInformation(tr("Dostępne są aktualizacje %1.").arg(xmlStr));
           }
        else
           {
             setUpdateDetails("");
             setHasUpdate(false);
             //CMessageBox::OkDialogInformation(tr("Brak aktualizacji."));
           }
        //exitCode= 0 , exitStatus= 0
        //out> "[0] Warning: Could not create lock file 'C:/Program Files (x86)/HelloWorld/lockmyApp15021976.lock':
        // (0x00000005)
        //<updates>
        //    <update version="0.3.0-1" name="The root component" size="175748"/>
        //</updates>
        //
        //"
        //err> ""
/*
        QString xmlStr;
        QStringList lines;
        bool enabled = false;
        lines = stdOutStr.split("\n");
        Q_FOREACH(auto line, lines)
        {
            if(line.startsWith("<updates>"))
            {
                enabled = true;
                xmlStr.append(line);
            }
            else if(line.endsWith("</updates>"))
            {
                xmlStr.append(line);
                break;
            }
            else if(enabled)
            {
                xmlStr.append(line);
            }
            else
            {

            }
        }
        if(xmlStr.length() > 0)
        {
            setUpdateDetails(xmlStr);
            setHasUpdate(true);
        }
*/


    }
    else if(exitCode == 1)
    {
        QByteArray stdErr = m_process.readAllStandardError();
        QString stdErrStr = QString::fromLocal8Bit(stdErr);
        DEBUG_WITH_LINE << "err>" << stdErrStr;

        //exitCode= 1 , exitStatus= 0
        //out> "[0] Warning: Could not create lock file 'C:/Program Files (x86)/HelloWorld/lockmyApp15021976.lock':
        // (0x00000005)
        //"
        //err> "There are currently no updates available.
        //"
    }
    setState(CMaintenanceTool::NotRunning);
}

void CMaintenanceTool::processError(QProcess::ProcessError error)
{
    DEBUG_WITH_LINE << error;
    setState(CMaintenanceTool::NotRunning);
}
