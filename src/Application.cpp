#include "Application.h"
#include <QProcessEnvironment>
#include <QLibraryInfo>
#include <QTranslator>
#include <QToolTip>
//#include "ExceptionSql.h"
//#include "ExceptionDialog.h"
#include "Style.h"
#include "MessageBox.h"



CApplication* CApplication::m_instance = NULL;

CApplication::CApplication(int & argc, char **argv) :
    QApplication(argc, argv), m_nologin(false), m_no_ask_exit(false)
{
    Q_ASSERT(m_instance == NULL);

    m_instance = this;

    m_system_palette = this->palette();

    this->initApplication(argc, argv);
    this->setAppInformation();
    this->initAppRegisterMetaType();

    if (argc > 1)
        {
            for(int i = 1; i < argc; i++)
                {
                    QString arg = QString(argv[i]);
                    if (arg == "-l")
                        {
                            m_nologin = true;
                            m_no_ask_exit = true;
                        }
                }
        }
    //this->run();
}

CApplication::~CApplication()
{
    //delete mainWindow;
    //delete logger;
    //delete owner;
}

CApplication* CApplication::getInstance()
{
    return m_instance;
}

bool CApplication::notify(QObject * receiver, QEvent * event)
{
    bool res=false;
    try
    {
       res = QApplication::notify(receiver, event);
    }

    catch(const QString & error)
    {
//        CExceptionDialog d(tr("Nieobsłużony wyjątek"), error);
//        d.exec();
        //abort();
    }
   catch (std::exception &e)
    {      
      CMessageBox::OkDialogCritical(QObject::tr("<p style='font-weight: bold; font-size: x-large; color: #f00'>Błąd krytyczny!</p><p>%1</p>").arg(e.what()));
      throw e;
    }
//    catch(CExceptionSql *e)
//    {
//        DEBUG_WITH_LINE << "Nieprzechwycony wyjątek w metodzie: " << e->getSourceMethod() << "\nKomunikat: " << e->getMessage();
//        QString msg = tr("Nieprzechwycony wyjątek w metodzie: ") + e->getSourceMethod() + QString("\n")+tr( "Komunikat: ") + e->getMessage();
//        CMessageBox::OkDialogCritical(QObject::tr("<p style='font-weight: bold; font-size: x-large; color: #f00'>Błąd krytyczny!</p><p>%1</p>").arg(msg));
//    }
    catch (...)
    {
      //TODO własne dialogi
      CMessageBox::OkDialogCritical(QObject::tr("<p>Nieznany błąd krytyczny!</p>"));
      //abort();
    }
   return res;
}

void CApplication::initApplication(int & argc, char **argv)
{

    QStringList tr_module_list = QStringList() << "qtbase_" << "designer_" << "qt_help_" << "limereport_";
//    if(argc > 1)
//    {
//        ArgumentsAnalyzer analyzer;
//        analyzer.analyze(argc, argv);
//    }
//    else
    {
      //ApplicationInfo appInfo;
      //appInfo.initAppliactionPath(argv[0]);
     Q_UNUSED(argc)
     Q_UNUSED(argv)

     QLocale::setDefault(QLocale(QLocale::Polish, QLocale::Poland));     

     setlocale(LC_ALL,"C");
     QProcessEnvironment::systemEnvironment().insert("LC_ALL", "C");

     QString translationPath = QApplication::applicationDirPath().remove("/bin");

     if (translationPath.contains("/bin"))
         translationPath.remove("/bin");

     translationPath.append("/translations");

     DEBUG_WITH_LINE << "Path to translations: " << QLibraryInfo::location(QLibraryInfo::TranslationsPath) << ", " << translationPath;
     DEBUG_WITH_LINE << "Locale name: " << QLocale::system().name();

     QString translationName = QLocale(QLocale::system().language()).name();
     DEBUG_WITH_LINE <<  "Language: " << QLocale(QLocale::system().language()).name();

     Q_FOREACH(auto tr_name, tr_module_list)
     {
         QTranslator qtTranslator;
         QString tr_filename = tr_name + translationName;
         DEBUG_WITH_LINE <<  "Tanslate filename: " << tr_filename;

         if (qtTranslator.load(tr_filename , QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
         {
             DEBUG_WITH_LINE << "Translation load sucess from path: " << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
             this->installTranslator(&qtTranslator);
         }
         else
         {
             if (qtTranslator.load(tr_filename, translationPath))
             {
                 DEBUG_WITH_LINE << "Translation load sucess from path: " << translationPath;
                 this->installTranslator(&qtTranslator);
             }
             else
             {
                 if(qtTranslator.isEmpty())
                     DEBUG_WITH_LINE << "Nie można załadować tłumaczenia Qt; część będzie po angielsku!";
             }
         }
     }
     Qt::LayoutDirection layoutDirection = QLocale(QLocale::system().language()).textDirection();
     this->setLayoutDirection(layoutDirection);

    //std::setlocale(LC_NUMERIC, "C"); // bo nie złapie przecinków dla floatów w editlajnach
     //Ładowanie Splash screen jakby był

      //Ustawienie stylu
      STYLE.setApplicationStyle(this);
//Dodanie ścieżek do bibliotek
      this->addLibraryPath("./");
      this->addLibraryPath("./lib/");
    }
}

void CApplication::setAppInformation()
{
    QCoreApplication::setOrganizationName(QString(APP_ORGANIZATION_NAME));
    QCoreApplication::setOrganizationDomain(QString(APP_ORGANIZATIO_DOMAIN));
    QCoreApplication::setApplicationName(QString(APP_NAME));
    QCoreApplication::setApplicationVersion(QString(APP_VERSION));

#if QT_VERSION <= 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif
}

int CApplication::run()
{
    if (m_nologin)
        return RET_OK;

//    CLoginDialogController login;

//    if (login.exec() == QDialog::Rejected)
//        {
//            return RET_ERROR;
//        }
//   return RET_OK;
}

void CApplication::logout()
{
    //delete mainWindow;
    //login();
}

void CApplication::initAppRegisterMetaType(void)
{
    qRegisterMetaType<Value>("Value");   //Wartość 2 miejsca po przecinku
    qRegisterMetaType<Price>("Price");   //Cena 2 miejsca po przecinku
    qRegisterMetaType<Amount>("Amount");  //Ilość 3 miejsca po przecinku
    qRegisterMetaType<Indicator>("Indicator"); //Wskaźnik 4 miejsca po przecinku
    qRegisterMetaType<Percent>("Percent"); //Wskaźnik 2 miejsca po przecinku
    qRegisterMetaType<Varchar255>("Varchar255"); //Varchar dł 255
    qRegisterMetaType<Timestamp>("Timestamp");   //Czas
    qRegisterMetaType<Color>("Color");           //Kolor
}

QString CApplication::toString(void)
{
  return QString("%1 %2").arg(APP_NAME).arg(APP_VERSION);
}

QString CApplication::iconName(void)
{
  return QString("%1").arg(APP_ICON);
}

void CApplication::loadColorScheme(EColorScheme color)
{
  switch (color)
  {
    case ColorDefault:
        QApplication::setPalette(m_system_palette);
    break;
    case ColorBright:
      loadPalette(QColor("#fafafa"), QColor("#333333"), QColor("#3276b1"), QColor("#635A5A"));
    break;
    case ColorDark:
      loadPalette(QColor("#222222"), QColor("#fafafa"), QColor("#3276b1"));
    break;
    case ColorBlue:
      loadPalette(QColor("#28385f"), QColor("#fafafa"), QColor("#28385f").darker());
    break;
    case ColorPurple:
      loadPalette(QColor("#55557f"), QColor("#fafafa"), QColor("#55557f").darker());
    break;
    case ColorMaroon:
      loadPalette(QColor("#400000"), QColor("#fafafa"), QColor("#3276b1"));
    break;
  }
}

void CApplication::loadPalette(QColor baseColor, QColor textColor, QColor highlightColor, QColor disabledText)
{
  QPalette palette = this->palette();

  QColor contrastColor;
  if(((highlightColor.red() * 0.299) + (highlightColor.green() * 0.587) + (highlightColor.blue() * 0.114)) > 146)
      contrastColor = QColor(Qt::black);
  else
      contrastColor = QColor(Qt::white);

  palette.setColor(QPalette::Normal, QPalette::Window, baseColor);
  palette.setColor(QPalette::Normal, QPalette::WindowText, textColor);
  palette.setColor(QPalette::Normal, QPalette::Base, baseColor);
  palette.setColor(QPalette::Normal, QPalette::AlternateBase, baseColor.lighter(110));
  palette.setColor(QPalette::Normal, QPalette::ToolTipBase, baseColor);
  palette.setColor(QPalette::Normal, QPalette::ToolTipText, textColor);
  palette.setColor(QPalette::Normal, QPalette::Text,textColor);
  palette.setColor(QPalette::Normal, QPalette::Button, baseColor);
  palette.setColor(QPalette::Normal, QPalette::ButtonText, textColor);
  palette.setColor(QPalette::Normal, QPalette::Highlight, highlightColor);
  palette.setColor(QPalette::Normal, QPalette::HighlightedText, contrastColor);

  palette.setColor(QPalette::Normal, QPalette::Light, baseColor);
  palette.setColor(QPalette::Normal, QPalette::Midlight, baseColor.darker(120));
  palette.setColor(QPalette::Normal, QPalette::Mid, QColor("#999999"));
  palette.setColor(QPalette::Normal, QPalette::Dark, QColor("#333333"));

  palette.setColor(QPalette::Normal, QPalette::Shadow, baseColor.darker(130));
  palette.setColor(QPalette::Normal, QPalette::BrightText, textColor.lighter(130));

  palette.setColor(QPalette::Disabled, QPalette::Window, baseColor.darker(105));
  palette.setColor(QPalette::Disabled, QPalette::WindowText, textColor.lighter(110));
  palette.setColor(QPalette::Disabled, QPalette::Base, baseColor.darker(115));
  palette.setColor(QPalette::Disabled, QPalette::AlternateBase, baseColor.lighter(120));
  palette.setColor(QPalette::Disabled, QPalette::ToolTipBase, QColor("#ffffff"));
  palette.setColor(QPalette::Disabled, QPalette::ToolTipText, QColor("#333333"));
  palette.setColor(QPalette::Disabled, QPalette::Text, disabledText);
  palette.setColor(QPalette::Disabled, QPalette::Button, baseColor.darker(105));
  palette.setColor(QPalette::Disabled, QPalette::ButtonText, textColor.lighter(130));
  palette.setColor(QPalette::Disabled, QPalette::Highlight, highlightColor.lighter(150));
  palette.setColor(QPalette::Disabled, QPalette::HighlightedText, contrastColor.lighter(110));

  palette.setColor(QPalette::Inactive, QPalette::Window, baseColor.darker(110));
  palette.setColor(QPalette::Inactive, QPalette::WindowText, textColor.lighter(120));
  palette.setColor(QPalette::Inactive, QPalette::Base, baseColor.darker(110));
  palette.setColor(QPalette::Inactive, QPalette::AlternateBase, baseColor.darker(110));
  palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, baseColor);
  palette.setColor(QPalette::Inactive, QPalette::ToolTipText, textColor);
  palette.setColor(QPalette::Inactive, QPalette::Text,textColor.lighter(130));
  palette.setColor(QPalette::Inactive, QPalette::Button, baseColor.darker(110));
  palette.setColor(QPalette::Inactive, QPalette::ButtonText, textColor.lighter(130));
  palette.setColor(QPalette::Inactive, QPalette::Highlight, highlightColor.lighter(150));
  palette.setColor(QPalette::Inactive, QPalette::HighlightedText, contrastColor.lighter(130));

  palette.setColor(QPalette::Inactive, QPalette::Light, baseColor.lighter(130));
  palette.setColor(QPalette::Inactive, QPalette::Midlight, baseColor.darker(120));
  palette.setColor(QPalette::Inactive, QPalette::Mid, QColor("#999999"));
  palette.setColor(QPalette::Inactive, QPalette::Dark, QColor("#333333"));

  palette.setColor(QPalette::Inactive, QPalette::Shadow, baseColor.darker(130));
  palette.setColor(QPalette::Inactive, QPalette::BrightText, textColor.lighter(130));

  QApplication::setPalette(palette);
}


QStringList CApplication::availableColors()
{
/*    QStringList list;
    QMetaEnum propEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator("EColorScheme"));
    for (int i = 0; i < propEnum.keyCount(); i++)
      {
        list.append(propEnum.key(i));
      }
    return list;
*/
  return QStringList() << tr("Systemowy") << tr("Jasny") << tr("Ciemny") << tr("Niebieski") << tr("Purpurowy") << tr("Kasztan");
}

void CApplication::updateAllWidgets()
{
    QPalette palette = this->palette();
    QFont font = this->font();
    QString style = this->styleSheet();

    Q_FOREACH(auto *widget, this->allWidgets())
    {
        widget->setPalette(palette);
        widget->setFont(font);
        widget->setStyleSheet(style);
    }
    font = QToolTip::font();
    QToolTip::setFont(font);
}

void CApplication::updatePalette(int color)
{
    loadColorScheme(static_cast<EColorScheme>(color));
    updateAllWidgets();
}

void CApplication::updateFont(const QFont &font)
{
    this->setFont(font);
    updateAllWidgets();
}

void CApplication::updateStyleSheet(int style)
{
    STYLE.changeApplicationStyle(static_cast<CStyle::StyleSheet>(style));
    STYLE.setApplicationStyle(this);
    updateAllWidgets();
}
