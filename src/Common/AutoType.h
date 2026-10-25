#ifndef CAUTOTYPE_H
#define CAUTOTYPE_H

#include <QObject>
#include <QStringList>
#include <QWidget>
#include "Global.h"
#include "AutoTypePlatform.h"

#include <memory>

class CAutoTypeAction;
class CAutoTypeExecutor;

typedef QPair<QString, QString> Entry;

class CAutoType : public QObject
{
    Q_OBJECT

public:
    QStringList windowTitles();
    void performAutoType(const Entry* entry, QWidget* hideWindow = nullptr,
                         const QString& customSequence = QString(), WId window = 0);
    bool registerGlobalShortcut(Qt::Key key, Qt::KeyboardModifiers modifiers);
    void unregisterGlobalShortcut();
    int callEventFilter(void* event);

    inline bool isAvailable()
    {
        if (m_platform_intrerface)
          return m_platform_intrerface->isAvailable();
        else
          return false;
    }

    // Types into other windows through \a platform (Strategy); main.cpp
    // creates the one for this system with createPlatform(), tests pass
    // CAutoTypePlatformTest.
    explicit CAutoType(std::unique_ptr<CAutoTypePlatform> platform, QObject* parent = nullptr);
    ~CAutoType();
    // The platform of this system (Factory Method).
    static std::unique_ptr<CAutoTypePlatform> createPlatform();

public Q_SLOTS:
    void performGlobalAutoType(const QList<Entry*>& entryList);

Q_SIGNALS:
    void globalShortcutTriggered();

private Q_SLOTS:
    void performAutoTypeFromGlobal(Entry* entry, const QString& sequence);
    void resetInAutoType();
    void unloadPlugin();

private:
    void loadPlugin(const QString& pluginPath);
    bool parseActions(const QString& sequence, const Entry* entry, QList<CAutoTypeAction*>& actions);
    QList<CAutoTypeAction*> createActionFromTemplate(const QString& tmpl, const Entry* entry);
    QString autoTypeSequence(const Entry* entry);

    bool m_inAutoType;
    Qt::Key m_currentGlobalKey;
    Qt::KeyboardModifiers m_currentGlobalModifiers;

    std::unique_ptr<CAutoTypePlatform> m_platform_intrerface;
    CAutoTypeExecutor* m_executor;
    WId m_windowFromGlobal;

    Q_DISABLE_COPY(CAutoType)
};

#endif // CAUTOTYPE_H
