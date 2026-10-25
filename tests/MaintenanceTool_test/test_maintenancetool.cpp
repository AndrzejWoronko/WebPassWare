#include <QObject>
#include <QString>
#include <QtTest>
#include <QDir>
#include <QFile>
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

private:
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

QTEST_MAIN(Test_MaintenanceTool)

#include "test_maintenancetool.moc"
