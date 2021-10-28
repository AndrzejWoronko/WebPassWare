#include "Style.h"
#include <QPainter>

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
            m_qssPath = QString("%1/%2").arg(STYLE_PATH, "blue");
        break;
        case GREY:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH, "grey");
        break;
        case GOOGLE:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH, "material-blue");
        break;
        case DARK:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH, "dark");
        break;
        case DARKORANGE:
            m_qssPath = QString("%1/%2").arg(STYLE_PATH, "darkorange");
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

QIcon CStyle::mergeIconAttribute(const QIcon &icon, const QString &attr)
{
    QIcon attrIcon;
    QString iconPath;

    iconPath = QString("%1/%2").arg(ICONS_PATH, getIconNameForAttribute(attr));
    attrIcon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
    QPixmap attrPixmap = attrIcon.pixmap(24, 24);
    QPixmap newPixmap = icon.pixmap(50, 50);

    QPainter painter(&newPixmap);
    painter.drawPixmap(50-24, 50-24, attrPixmap);
    //icon.addPixmap(newPixmap, QIcon::Normal, QIcon::Off);
    // Create new icon
    return QIcon(newPixmap);
}

QPixmap CStyle::mergePixmapAttribute(const QPixmap &pixmap, const QString &attr)
{
    QIcon attrIcon;
    QString iconPath;

    iconPath = QString("%1/%2").arg(ICONS_PATH, getIconNameForAttribute(attr));
    attrIcon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);
    QPixmap attrPixmap = attrIcon.pixmap(24, 24);

    QPixmap newPixmap = pixmap;
    QPainter painter(&newPixmap);
    painter.drawPixmap(50-24, 50-24, attrPixmap);
    return newPixmap;
}

QIcon CStyle::iconFromStyle(const QString &iconName)
{
    QIcon icon;
    QString iconPath;
    QString iconNameTmp = iconName;
    QString iconNameAttr;

    iconNameAttr.clear();
    if (iconNameTmp.contains('|'))
       {
         QStringList list = iconNameTmp.split('|');
         iconNameTmp = list.at(0);
         iconNameAttr =  list.at(1);
       }

    if (iconNameTmp.contains('.'))
        iconPath = QString("%1/%2").arg(ICONS_PATH, iconNameTmp);
    else
        iconPath = QString("%1/%2.png").arg(ICONS_PATH, iconNameTmp);

    icon.addFile(iconPath, QSize(), QIcon::Normal, QIcon::Off);

    if (!iconNameAttr.isEmpty()) //Merge icons
    {
         icon = mergeIconAttribute(icon, iconNameAttr);
    }
    return icon;
}

bool CStyle::iconFromStyleExist(const QString &iconName)
{
    QString iconPath;   
    QString iconNameTmp = iconName;

    if (iconNameTmp.contains('|'))
       {
         iconNameTmp = iconNameTmp.split('|').at(0);
       }

    if (iconNameTmp.contains('.'))
        iconPath = QString("%1/%2").arg(ICONS_PATH, iconNameTmp);
    else
        iconPath = QString("%1/%2.png").arg(ICONS_PATH, iconNameTmp);

    return  QFile::exists(iconPath);
}

QString CStyle::iconFromStylePath(const QString &iconName)
{
    QString iconPath;
    QString iconNameTmp = iconName;

    if (iconNameTmp.contains('|'))
       {
         iconNameTmp = iconNameTmp.split('|').at(0);
       }

    if (iconNameTmp.contains('.'))
        iconPath = QString("%1/%2").arg(ICONS_PATH, iconNameTmp);
    else
        iconPath = QString("%1/%2.png").arg(ICONS_PATH, iconNameTmp);
    return  iconPath;
}

QPixmap CStyle::pixmapFromStyle(const QString &pixmapName)
{
     QString pixmapPath;
     QPixmap pixMap;
     QString pixmapNameTmp = pixmapName;
     QString pixmapNameAttr;

     pixmapNameAttr.clear();
     if (pixmapNameTmp.contains('|'))
        {
            QStringList list = pixmapNameTmp.split('|');
            pixmapNameTmp = list.at(0);
            pixmapNameAttr =  list.at(1);
        }

     if (pixmapNameTmp.contains('.'))
         pixmapPath = QString("%1/%2").arg(ICONS_PATH, pixmapNameTmp);
     else
         pixmapPath = QString("%1/%2.png").arg(ICONS_PATH, pixmapNameTmp);

     pixMap.load(pixmapPath);

     if (!pixmapNameAttr.isEmpty()) //Merge pixmaps
     {
          pixMap = mergePixmapAttribute(pixMap, pixmapNameAttr);
     }
     return pixMap;
}

QString CStyle::getIconNameForAttribute(CStyle::IconAttributes attr)
{
    switch (attr)
    {
        case PLUS:
        case ADD:
            return "Plus.png";
        case MINUS:
            return "Minus.png";
        case EDIT:
            return "Edit.png";
        case DELETE:
            return "Delete.png";
        case DENIED:
            return "Denied.png";
        case INFO:
            return "Info.png";
        case WARNING:
            return "Warn.png";
        case QUESTION:
            return "Question.png";
        case ERROR:
            return "Error.png";
        case SORT_ASC:
            return "Sort_asc.png";
        case SORT_DESC:
            return "Sort_desc.png";
        case LIGHTENING:
            return "Lightning.png";
        case DISK:
        case SAVE:
            return "Save.png";
        default:
            DEBUG_WITH_LINE << "Unhandled icon attribute:" << attr;
    }
    return QString();
}

QString CStyle::getIconNameForAttribute(const QString &attr)
{
    if (attr.toUpper() == "PLUS" || attr.toUpper() == "ADD")
        return getIconNameForAttribute(CStyle::PLUS);
    else if (attr.toUpper() == "MINUS")
        return getIconNameForAttribute(CStyle::MINUS);
    else if (attr.toUpper() == "EDIT")
        return getIconNameForAttribute(CStyle::EDIT);
    else if (attr.toUpper() == "DELETE")
        return getIconNameForAttribute(CStyle::DELETE);
    else if (attr.toUpper() == "DENIED")
        return getIconNameForAttribute(CStyle::DENIED);
    else if (attr.toUpper() == "INFO")
        return getIconNameForAttribute(CStyle::INFO);
    else if (attr.toUpper() == "WARNING")
        return getIconNameForAttribute(CStyle::WARNING);
    else if (attr.toUpper() == "QUESTION")
        return getIconNameForAttribute(CStyle::QUESTION);
    else if (attr.toUpper() == "ERROR")
        return getIconNameForAttribute(CStyle::ERROR);
    else if (attr.toUpper() == "SORT_ASC")
        return getIconNameForAttribute(CStyle::SORT_ASC);
    else if (attr.toUpper() == "SORT_DESC")
        return getIconNameForAttribute(CStyle::SORT_DESC);
    else if (attr.toUpper() == "DISK" || attr.toUpper() == "SAVE")
        return getIconNameForAttribute(CStyle::DISK);
    else if (attr.toUpper() == "LIGHTENING")
        return getIconNameForAttribute(CStyle::LIGHTENING);
    else
        return getIconNameForAttribute(CStyle::NONE);

/*    QMetaEnum iconAttEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator("IconAttributes"));

    for (int i = 0; i < iconAttEnum.keyCount(); i++)
      {
         if (QString(iconAttEnum.key(i)).toUpper() == attr.toUpper())
         {
             return getIconNameForAttribute(static_cast<CStyle::IconAttributes>(iconAttEnum.value(i)));
         }
      }
*/
    return QString();
}

QStringList CStyle::availableStyles()
{
  return QStringList() << QObject::tr("System default") << QObject::tr("Blue") << QObject::tr("Gray") << QObject::tr("Material-Google") << QObject::tr("Dark") << QObject::tr("Dark Orange");
}
