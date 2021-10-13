#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "Global.h"
#include "AbstractCloseDialog.h"


class CAboutDialog : public CAbstractCloseDialog
{

    ADD_PTR_PROPERTY(CFormEditorField, m_application_info, getApplicationInfo)
    ADD_PTR_PROPERTY(CFormEditorField, m_author_info, getAuthorInfo)
    ADD_PTR_PROPERTY(CFormEditorField, m_license_info, getLicenseInfo)
    ADD_PTR_PROPERTY(CFormEditorField, m_system_info, getSystemInfo)

    ADD_PTR_PROPERTY(QWidget, m_tabInfo, getTabInfo)
    ADD_PTR_PROPERTY(QWidget, m_tabAuthor, getTabAuthor)
    ADD_PTR_PROPERTY(QWidget, m_tabLicense, getTabLicense)
    ADD_PTR_PROPERTY(QWidget, m_tabSysInfo, getSysInfo)

    ADD_PTR_PROPERTY(QTabWidget, m_tabWidget, getTabWidget)


public:

    CAboutDialog(QWidget *parent = NULL);
    ~CAboutDialog();
    //Funkcja tworząca pola
    virtual void addFields();

    //Funkcja umieszczająca pola na formatce
    virtual void addComponents();

private:

    CGridLayout *m_infoLayout;
    CGridLayout *m_authorLayout;
    CGridLayout *m_licenseLayout;
    CGridLayout *m_sysinfoLayout;

    void addAllInfoComponents();
    void addAllAuthorComponents();
    void addAllLicenseComponents();

    void createTabWidget();
    void setValues();
};

#endif // ABOUTDIALOG_H
