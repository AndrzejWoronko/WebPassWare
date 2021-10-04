#include "Style.h"

CStyle::CStyle()
{
    initActualStyle();
}

CStyle::~CStyle()
{

}

void CStyle::initActualStyle()
{
    m_actualStyle = static_cast<StyleSheet>(SETT.getValue(SETTINGS_GUI_STYLE).toInt());

    switch (m_actualStyle)
    {
        case BLUE:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH).arg("blue");
        break;
        case GREY:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH).arg("grey");
        break;
        case GOOGLE:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH).arg("material-blue");
        break;
        case DARK:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH).arg("dark");
        break;
        case DARKORANGE:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH).arg("darkorange");
        break;
        default:
            m_qssPath = QString();
        break;
    }
}

void CStyle::setApplicationStyle(QApplication *app)
{
    if (m_qssPath.isEmpty())
    {
        app->setStyleSheet(QString(""));
    }
    else
    {
        QFile qss(m_qssPath);
        if (qss.open(QFile::ReadOnly))
           {
                QString readall = QString(qss.readAll());
                app->setStyleSheet(readall);
                qss.close();
           }
    }
}

void CStyle::changeApplicationStyle(StyleSheet newStyle)
{
    setStyleSheet(newStyle);
    SETT.setValue(SETTINGS_GUI_STYLE, QVariant(newStyle));
    initActualStyle();
}

void  CStyle::setStyleSheet(StyleSheet newStyle)
{
    m_actualStyle = newStyle;
}

QString CStyle::getQssPath()
{
    return m_qssPath;
}


bool CStyle::isBlueStyleSheet()
{
    return m_actualStyle == BLUE;
}

bool CStyle::isGreyStyleSheet()
{
    return m_actualStyle == GREY;
}

bool CStyle::isStandardStyleSheet()
{
    return m_actualStyle == STANDARD;
}

QIcon CStyle::iconFromStyle(const QString &iconName)
{
    QIcon icon;
    QString iconPath;

    if (iconName.contains('.'))
        iconPath = QString("%1/%2").arg(ICONS_PATH).arg(iconName);
    else
        iconPath = QString("%1/%2.png").arg(ICONS_PATH).arg(iconName);

    icon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
    return icon;
}

bool CStyle::iconFromStyleExist(const QString &iconName)
{
    QString iconPath;

    if (iconName.contains('.'))
        iconPath = QString("%1/%2").arg(ICONS_PATH).arg(iconName);
    else
        iconPath = QString("%1/%2.png").arg(ICONS_PATH).arg(iconName);

    return  QFile::exists(iconPath);
}

QString CStyle::iconFromStylePath(const QString &iconName)
{
    QString iconPath;

    if (iconName.contains('.'))
        iconPath = QString("%1/%2").arg(ICONS_PATH).arg(iconName);
    else
        iconPath = QString("%1/%2.png").arg(ICONS_PATH).arg(iconName);
    return  iconPath;
}

QPixmap CStyle::pixmapFromStyle(const QString &pixmapName)
{
     QString pixmapPath;
     QPixmap pixMap;

     if (pixmapName.contains('.'))
         pixmapPath = QString("%1/%2").arg(ICONS_PATH).arg(pixmapName);
     else
         pixmapPath = QString("%1/%2.png").arg(ICONS_PATH).arg(pixmapName);

     pixMap.load(pixmapPath);
     return  pixMap;
}

QStringList CStyle::availableStyles()
{
  return QStringList() << QObject::tr("System default") << QObject::tr("Blue") << QObject::tr("Gray") << QObject::tr("Material-Google") << QObject::tr("Dark") << QObject::tr("Dark Orange");
}
