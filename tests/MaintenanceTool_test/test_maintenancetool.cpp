#include <QObject>
#include <QString>
#include <QtTest>
#include <QDir>
#include <QFile>
#include <QSignalSpy>
#include <QTemporaryDir>

#include "MaintenanceTool.h"

// The updater is the maintenancetool of the Qt Installer Framework next to
// the program (the working directory). Installs without it (the Debian
// package) must not offer an update check that can only fail.
class Test_MaintenanceTool : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void test_unavailableWithoutTool();
    void test_availableWithExecutableTool();
    void test_checkReportsUpdates();
    void test_checkReportsNoUpdates();
    void test_checkWithoutToolFinishes();

private:
    // Replaces the tool with a script that prints \a output.
    void writeTool(const QByteArray &output);

    QTemporaryDir m_dir;
    QString m_oldDir;
};

void Test_MaintenanceTool::initTestCase()
{
    QVERIFY(m_dir.isValid());
    m_oldDir = QDir::currentPath();
    QVERIFY(QDir::setCurrent(m_dir.path()));
}

void Test_MaintenanceTool::cleanupTestCase()
{
    QDir::setCurrent(m_oldDir);
}

void Test_MaintenanceTool::test_unavailableWithoutTool()
{
    QVERIFY(!CMaintenanceTool::isAvailable());
}

void Test_MaintenanceTool::test_availableWithExecutableTool()
{
    QFile tool(CMaintenanceTool::toolPath());
    QVERIFY(tool.open(QIODevice::WriteOnly));
    tool.write("#!/bin/sh\n");
    tool.close();
    QVERIFY(!CMaintenanceTool::isAvailable());

    QVERIFY(tool.setPermissions(tool.permissions() | QFile::ExeOwner));
    QVERIFY(CMaintenanceTool::isAvailable());
}

void Test_MaintenanceTool::writeTool(const QByteArray &output)
{
    QFile tool(CMaintenanceTool::toolPath());
    QVERIFY(tool.open(QIODevice::WriteOnly | QIODevice::Truncate));
    tool.write("#!/bin/sh\nsleep 0.2\nprintf '%s\\n' '" + output + "'\n");
    tool.close();
    QVERIFY(tool.setPermissions(tool.permissions() | QFile::ExeOwner));
}

void Test_MaintenanceTool::test_checkReportsUpdates()
{
    writeTool("<updates><update name=\"WebPassWare\" version=\"0.2.0\"/></updates>");
    CMaintenanceTool tool;
    QSignalSpy finished(&tool, &CMaintenanceTool::checkFinished);

    tool.checkUpdate();
    // The check runs in the background: the call returns at once.
    QCOMPARE(finished.count(), 0);
    QVERIFY(finished.wait(5000));
    QCOMPARE(finished.first().first().toBool(), true);
    QVERIFY(tool.hasUpdate());
    QVERIFY(tool.updateDetails().contains("0.2.0"));
    QCOMPARE(tool.state(), CMaintenanceTool::NotRunning);
}

void Test_MaintenanceTool::test_checkReportsNoUpdates()
{
    writeTool("There are currently no updates available.");
    CMaintenanceTool tool;
    QSignalSpy finished(&tool, &CMaintenanceTool::checkFinished);

    tool.checkUpdate();
    QVERIFY(finished.wait(5000));
    QCOMPARE(finished.first().first().toBool(), false);
    QVERIFY(!tool.hasUpdate());
}

void Test_MaintenanceTool::test_checkWithoutToolFinishes()
{
    QFile::remove(CMaintenanceTool::toolPath());
    CMaintenanceTool tool;
    QSignalSpy finished(&tool, &CMaintenanceTool::checkFinished);

    tool.checkUpdate();
    QVERIFY(finished.count() == 1 || finished.wait(5000));
    QCOMPARE(finished.first().first().toBool(), false);
}

QTEST_MAIN(Test_MaintenanceTool)

#include "test_maintenancetool.moc"
