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
    enum StyleSheet {
        STANDARD = 0,
        BLUE,
        GREY,
        GOOGLE,
        DARK,
        DARKORANGE
    };

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

};

#endif // STYLE_H
