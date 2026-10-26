#ifndef TESTENVIRONMENT_H
#define TESTENVIRONMENT_H

#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QUuid>

// Construct before the application: native settings and data paths ignore XDG
// overrides on Windows and macOS. The unique file name adds a second safeguard.
class TestEnvironment
{
public:
    TestEnvironment()
        : m_databaseName(QString("test-vault-%1.db").arg(QUuid::createUuid().toString(QUuid::Id128)))
    {
        QStandardPaths::setTestModeEnabled(true);
        if (!m_dir.isValid())
        {
            return;
        }
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, settingsPath());
        QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, m_dir.filePath("system-config"));
    }

    bool isValid() const { return m_dir.isValid(); }
    QString settingsPath() const { return m_dir.filePath("config"); }
    QString databaseName() const { return databasePath(); }

    QString databasePath() const
    {
        return QDir::cleanPath(m_dir.filePath(m_databaseName));
    }

    bool ownsDatabasePath(const QString &path) const
    {
        return isValid() && QStandardPaths::isTestModeEnabled() && QDir::cleanPath(path) == databasePath();
    }

private:
    QTemporaryDir m_dir;
    const QString m_databaseName;
};

#endif // TESTENVIRONMENT_H
