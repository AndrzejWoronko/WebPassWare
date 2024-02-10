#include "AboutDialog.h"
#include "ApplicationSettings.h"
#include <QSysInfo>

CAboutDialog::CAboutDialog(QWidget *parent) : CAbstractCloseDialog(QString("ABOUT_DIALOG"), parent)
{
    this->setWindowTitle(tr("O programie"));
}

void CAboutDialog::addFields()
{
    m_application_info = new CFormEditorField(QString("m_application_info"), tr("Informacje o aplikacji"), "", true);
    m_author_info = new CFormEditorField(QString("m_author_info"), tr("Autor"), "", true);
    m_license_info = new CFormEditorField(QString("m_license_info"), tr("Licencje"), "", true);
    m_system_info = new CFormEditorField(QString("m_system_info"), tr("Informacje systemowe"), "", true);
}


void CAboutDialog::createTabWidget()
{
    m_tabWidget = new QTabWidget(getWidget().get());
    m_tabWidget->addTab(m_tabInfo = new QWidget(), QString(APP_NAME));
    m_tabWidget->addTab(m_tabAuthor = new QWidget(), tr("Autor"));
    m_tabWidget->addTab(m_tabLicense = new QWidget(),tr("Licencje"));
    m_tabWidget->addTab(m_tabSysInfo = new QWidget(),tr("Informacje systemowe"));

    m_tabInfo->setLayout(m_infoLayout = new CGridLayout());
    m_tabAuthor->setLayout(m_authorLayout = new CGridLayout());
    m_tabLicense->setLayout(m_licenseLayout = new CGridLayout());
    m_tabSysInfo->setLayout(m_sysinfoLayout = new CGridLayout());
}

void CAboutDialog::addComponents()
{
   createTabWidget();
   setValues();
   CForm::setComponents(m_infoLayout, m_application_info, QString(), 0, 0, 6, 8);
   CForm::setComponents(m_authorLayout, m_author_info, QString(), 0, 0, 6, 8);
   CForm::setComponents(m_licenseLayout, m_license_info, QString(), 0, 0, 6, 8);
   CForm::setComponents(m_sysinfoLayout, m_system_info, QString(), 0, 0, 6, 8);
   this->setDialogWidget(m_tabWidget);
}

void CAboutDialog::setValues()
{

m_application_info->setHtml(tr( "<html><head/><body>"
                               "<p align=\"left\"><span style=\" font-size:11pt; font-weight:600;\">%1</span></p>"
                               "<p align=\"left\">%2</p>"
                               "<p align=\"left\">Strona www: <a href=\"%3\"><span style=\" text-decoration: underline; color:#0000ff;\">%3</span></a></p>"
                               "<p align=\"left\">Copyright © 2022 %4<br/></p>"
                               "<p align=\"left\">Wersja programu: %5</p>"
                               "<p align=\"left\">Wersja Qt: %6</p>"                               
                               "<p align=\"left\">Ścieżka do ustawień: %8</p>"
                               "<p align=\"left\">Katalog roboczy: %9</p>"
                               "<p align=\"left\">Ścieżka do aplikacji: %10</p>"
                               "<p align=\"left\">Ikony: <a href=\"https://icons8.com\"><span style=\" text-decoration: underline; color:#0000ff;\">https://icons8.com</span></a></p>"
                               "</body></html>")
                       .arg(APP_NAME)
                       .arg(tr("Program do przechowywania haseł do serwisów www / poczty itp. "))
                       .arg(APP_ORGANIZATIO_DOMAIN)
                       .arg(APP_ORGANIZATION_NAME)
                       .arg(APP_VERSION)
                       .arg(QT_VERSION_STR)                       
                       .arg(SETT.getSettings()->fileName())
                       .arg(QDir::currentPath())
                       .arg(QCoreApplication::applicationDirPath())
                       );
//m_application_info->setOpenExternalLinks(true);

m_license_info->setText(
tr("UWAGA!!!\n") +
tr("Ten program komputerowy dostarczany jest przez autora w formie \"takiej, jaki jest\". ") +
tr("Autor nie udziela żadnej gwarancji oraz rękojmi, że program będzie działać ") +
tr("prawidłowo, jest odpowiedniej jakości oraz że spełni oczekiwania ") +
tr("użytkownika. Autor nie odpowiada za jakiekolwiek straty wynikające z użytkowania ") +
tr("programu, w tym utratą spodziewanych korzyści, danych, informacji ") +
tr("gospodarczych lub koszt urządzeń lub programów zastępczych."));

m_author_info->setHtml(tr("<html><head/><body>"
                           "<p align=\"left\"><span style=\" font-size:11pt; font-weight:600;\">Andrzej Woronko</span></p>"
                           "<p align=\"left\"><a href=\"mailto:andrzej.woronko@gmail.com\"><span style=\" text-decoration: underline; color:#0000ff;\">andrzej.woronko@gmail.com</span></a></p>"
                           "</body></html>"));
//m_author_info->setOpenExternalLinks(true);

QSysInfo systemInfo;

m_system_info->setHtml(tr("<html><head/><body>"
                        "<p align=\"left\">System operacyjny: %1</p>"
                        "<p align=\"left\">Użycie pamięci: %2</p>"
                        "<p align=\"left\">Architektura kompilacji CPU: %3</p>"
                        "<p align=\"left\">Architektura CPU: %4</p>"
                        "<p align=\"left\">Kernel typ: %5 wersja: %6</p>"
                        "<p align=\"left\">Nazwa komputera: %7</p>"
                        "<p align=\"left\">Kolejność bajtów: %8</p>"
                        "</body></html>")
                       .arg(systemInfo.prettyProductName())
                       .arg(Tools::getMemoryUsageToString())
                       .arg(systemInfo.buildCpuArchitecture())
                       .arg(systemInfo.currentCpuArchitecture())
                       .arg(systemInfo.kernelType())
                       .arg(systemInfo.kernelVersion())
                       .arg(systemInfo.machineHostName())
                       .arg(systemInfo.buildAbi())
                       );

/*
qDebug()<< "Product Type: " << systemInfo.productType();
qDebug()<< "Product Version: " << systemInfo.productVersion();
*/
}
