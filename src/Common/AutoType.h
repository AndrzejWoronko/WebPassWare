#ifndef CAUTOTYPE_H
#define CAUTOTYPE_H

#include <QObject>
#include <QStringList>
#include <QWidget>
#include "Global.h"

class CAutoTypeAction;
class CAutoTypeExecutor;
class CAutoTypePlatform;

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
          return true;
        else
          return false;
    }

    static CAutoType* instance();
    static void createTestInstance();

public Q_SLOTS:
    void performGlobalAutoType(const QList<Entry*>& entryList);

Q_SIGNALS:
    void globalShortcutTriggered();

private Q_SLOTS:
    void performAutoTypeFromGlobal(Entry* entry, const QString& sequence);
    void resetInAutoType();
    void unloadPlugin();

private:
    explicit CAutoType(QObject* parent = nullptr, bool test = false);
    ~CAutoType();
    void loadPlugin(const QString& pluginPath);
    bool parseActions(const QString& sequence, const Entry* entry, QList<CAutoTypeAction*>& actions);
    QList<CAutoTypeAction*> createActionFromTemplate(const QString& tmpl, const Entry* entry);
    QString autoTypeSequence(const Entry* entry);

    bool m_inAutoType;
    Qt::Key m_currentGlobalKey;
    Qt::KeyboardModifiers m_currentGlobalModifiers;

    CAutoTypePlatform* m_platform_intrerface;
    CAutoTypeExecutor* m_executor;
    WId m_windowFromGlobal;
    static CAutoType* m_instance;

    Q_DISABLE_COPY(CAutoType)
};

inline CAutoType* autoType() {
    return CAutoType::instance();
}

#endif // CAUTOTYPE_H
