#include <stdexcept>
#include <QApplication>
#include <QSplashScreen>
#include <QtGui>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QThread>
#include <unistd.h>
#include "Global.h"
#include "Logger.h"
#include "Style.h"
//#include "databasetablecheck.h"
#include "Application.h"
//#include "BackupAutoRemover.h"
#include "WebPassWareMainWindow.h"
#include "Database.h"
#include "ModelTableCheck.h"

// Projekty wzorcowe
//https://github.com/orgs/keepassxreboot/repositories
//https://github.com/orgs/keepassx
//https://github.com/Martchus/passwordmanager

int main(int argc, char **argv)
{
    bool check_tables = false;

    if (argc > 1)
    {
        for(int i = 1; i < argc; i++)
        {
            QString arg = QString(argv[i]);
            if(arg == "-v" || arg == "--version")
                {
                    DEBUG_WITH_LINE << "Wersja: " << APP_NAME << " " << APP_VERSION;
                    exit(0);
                }
            if(arg == "-c" || arg == "--check_tables")
                {
                    check_tables = true;
                }
        }
    }

//    if (d.exists(APP_DIR))
//        QDir::setCurrent(QString(APP_DIR));
//    else
//        QDir::setCurrent("./");

    CApplication *a = NULL;
    CWebPassWareMainWindow *w = NULL;

    a = new CApplication(argc, argv);

    QDir d(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACOS
    d.cdUp();    /* Fix this on Mac because of the .app folder, */
    d.cdUp();    /* which means that the actual executable is   */
    d.cdUp();    /* three levels deep. Grrr.                    */
#else
    if (d.absolutePath().contains("bin"))
        d.cdUp(); /*Application is in applicationDirPath() not in bin */
#endif
    QDir::setCurrent(d.absolutePath());
    DEBUG_WITH_LINE << "Katalog roboczy ustawiony na: " << QDir::currentPath();

    DEBUG_WITH_LINE << "Katalog aplikacji: " << QCoreApplication::applicationDirPath();
    DEBUG_WITH_LINE << "Plik aplikacji: " << QCoreApplication::applicationFilePath();
    DEBUG_WITH_LINE << "Katalog blibliotek: " << QCoreApplication::libraryPaths();

    if (a->run() == RET_ERROR)
       {
          //delete a;
         return 0;
       }
     CStyle::getInstance().setApplicationStyle(a);

/*
#if QT_VERSION >= 0x040400
  // This is the correct place for this call but on versions less
  // than 4.4 it causes a crash for an unknown reason so it is
  // called later on earlier versions.
  QCoreApplication::addLibraryPath(QString(PROGRAM_DIR));
#endif
 */
 // sprawdzenie czy jest katalog instalacji jeżeli nie ma to ustawia na bieżący


    //MainWindow1 w1;

    // Splash screen jakby był 
//    else
       {        
            a->processEvents();
            //DB.initConnection();
            //Sprawdzenie i otwarcie bazy danych
            //DB.createDatabase(DB.getMBaseName());

            if (check_tables)
               {
                 //Sprawdzenie/założenie tabel
                 if (DB.isConnected())
                    {
                      ModelTableCheck::checkAllTables();
                    }
                 //DataBaseTableCheck::checkAllTables();
                 //Init domyślnych wartości
                 //DataBaseTableCheck::initDefaultValues();
               }

            w = new CWebPassWareMainWindow();
            w->centerAndResize();
            //w->restoreDialogState();
            w->show();
            //CBackupAutoRemover::getInstance().checkAndRemoveOldBackups();
       }
    //logs(QObject::tr("Uruchomienie programu"));
    LOG_APP << QObject::tr("Uruchomienie programu");
    //w1.show();
    a->exec();
    if (w)
        delete w;

    return 0;
}
