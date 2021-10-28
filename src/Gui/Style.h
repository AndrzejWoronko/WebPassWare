#ifndef STYLE_H
#define STYLE_H

#include "Global.h"
#include <QApplication>
#include <QIcon>
#include <QStyle>
#include "Singleton.h"
#include "ApplicationSettings.h"

#define STYLE CStyle::getInstance()

class CStyle : public Singleton<CStyle>
{

public:
    enum StyleSheet
    {
        STANDARD = 0,
        BLUE,
        GREY,
        GOOGLE,
        DARK,
        DARKORANGE
    };

    Q_ENUMS(StyleSheet)

    enum IconAttributes
    {
        NONE,
        PLUS,
        ADD,
        MINUS,
        EDIT,
        DELETE,
        DENIED,
        INFO,
        WARNING,
        QUESTION,
        ERROR,
        SORT_ASC,
        SORT_DESC,
        LIGHTENING,
        DISK,        
        SAVE
    };
    Q_ENUMS(IconAttributes)

private:

    StyleSheet m_actualStyle;
    QString m_qssPath;

public:
    CStyle();
    ~CStyle();

    void setStyleSheet(StyleSheet newStyle);
    void setApplicationStyle(QApplication *app);
    void initActualStyle();
    QString getQssPath();

    bool isBlueStyleSheet();
    bool isGreyStyleSheet();
    bool isStandardStyleSheet();
    void changeApplicationStyle(StyleSheet newStyle);

    static QStringList availableStyles();

    static QIcon iconFromStyle(const QString &iconName);
    static bool iconFromStyleExist(const QString &iconName);
    static QString iconFromStylePath(const QString &iconName);
    static QPixmap pixmapFromStyle(const QString &pixmapName);


private:
    static QString getIconNameForAttribute(CStyle::IconAttributes attr);
    static QString getIconNameForAttribute(const QString &attr);
    static QIcon mergeIconAttribute(const QIcon &icon, const QString &attr);
    static QPixmap mergePixmapAttribute(const QPixmap &pixmap, const QString &attr);

};

#endif // STYLE_H
