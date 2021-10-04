#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QtGui>
#include <QTranslator>

#include "Global.h"
#include "ApplicationSettings.h"

#define APPI CApplication::getInstance()

typedef QHash<QString, QTranslator*> Translators;

class CApplication : public QApplication
{
    Q_OBJECT

    Q_ENUMS(EColorScheme)

    QPalette m_system_palette;
    bool m_nologin;
    bool m_no_ask_exit;

    void loadPalette(QColor baseColor, QColor textColor, QColor highlightColor, QColor disabledText = QColor(Qt::gray));

protected:
    static CApplication* m_instance;

public:
    enum EColorScheme
    {
      ColorDefault = 0,
      ColorBright,
      ColorDark,
      ColorBlue,
      ColorPurple,
      ColorMaroon,
    };

    CApplication(int & argc, char **argv);
    ~CApplication();
    static CApplication *getInstance();

    void initApplication(int & argc, char **argv);
    bool notify(QObject * receiver, QEvent * event);

    static void setAppInformation();
    static void initAppRegisterMetaType(void);

    static QString toString(void);

    static QString iconName(void);

    int run();

//Ustawienia kolorów

    void loadColorScheme(EColorScheme color);

    static QStringList availableColors(void);

    bool getNoAskExit(void) {return m_no_ask_exit;}

signals:

public slots:
    void logout();
    void updateAllWidgets();
    void updateFont(const QFont &font);
    void updatePalette(int);
    void updateStyleSheet(int style);
};

#endif // APPLICATION_H
