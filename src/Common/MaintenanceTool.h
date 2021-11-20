#ifndef CMAINTENANCETOOL_H
#define CMAINTENANCETOOL_H


#include "Global.h"
#include <QProcess>

/*
Usage

CMaintenanceTool updater;

updater.checkUpdate();

QProcess process;
process.start("maintenancetool --checkupdates");

// Wait until the update tool is finished
process.waitForFinished();

if(process.error() != QProcess::UnknownError)
{
    qDebug() << "Error checking for updates";
    return false;
}

// Read the output
QByteArray data = process.readAllStandardOutput();

// No output means no updates available
// Note that the exit code will also be 1, but we don't use that
// Also note that we should parse the output instead of just checking if it is empty if we want specific update info
if(data.isEmpty())
{
    qDebug() << "No updates available";
    return false;
}

// Call the maintenance tool binary
// Note: we start it detached because this application need to close for the update
QStringList args("--updater");
bool success = QProcess::startDetached("maintenancetool", args);

// Close the application
qApp->closeAllWindows();

￼


In the latest Qt Installer Framework 4.1 --checkupdates returns nothing, use ch or check-updates instead.

Commands:
    in, install - install default or selected packages - <pkg ...>
    ch, check-updates - show available updates information on maintenance tool
    up, update - update all or selected packages - <pkg ...>
    rm, remove - uninstall packages and their child components - <pkg ...>
    li, list - list currently installed packages - <regexp>
    se, search - search available packages - <regexp>
    co, create-offline - create offline installer from selected packages - <pkg ...>
    pr, purge - uninstall all packages and remove entire program directory

*/

class CMaintenanceTool : public QObject
{
    Q_OBJECT

    Q_ENUMS(ProcessState)
    Q_ENUMS(StartMode)

    Q_PROPERTY(ProcessState m_state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool m_hasUpdate READ hasUpdate NOTIFY hasUpdateChanged)
    Q_PROPERTY(QString m_updateDetails READ updateDetails NOTIFY updateDetailsChanged)

public:
    explicit CMaintenanceTool(QObject *parent = 0);

    enum ProcessState
    {
        NotRunning,
        Running
    };
    enum StartMode
    {
        CheckUpdate,
        Updater
    };

    ProcessState state() const;
    bool hasUpdate() const;
    QString updateDetails() const;

signals:
    void stateChanged(ProcessState state);
    void hasUpdateChanged(bool hasUpdate);
    void updateDetailsChanged(const QString &updateDetails);

public slots:
    void checkUpdate();
    void startUpdate();
    void startMaintenanceTool(StartMode mode = Updater);

private slots:
    void setHasUpdate(bool hasUpdate);
    void setUpdateDetails(const QString &updateDetails);

    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError error);

private:
    ProcessState m_state;
    bool m_hasUpdate;
    QString m_updateDetails;
    QProcess m_process;

    void setState(ProcessState state);
};

#endif // CMAINTENANCETOOL_H
