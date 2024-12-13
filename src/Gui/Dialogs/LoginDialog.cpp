#include "LoginDialog.h"
#include "Application.h"
#include "Database.h"
#include "MessageBox.h"

CLoginDialog::CLoginDialog(const QString &title, const QString &iconName, QWidget *parent) : CDialog(parent), CAbstractDialogStateManager()
{
    if (!iconName.isEmpty())
    {
        if (CStyle::iconFromStyleExist(iconName))
            this->setWindowIcon(CStyle::iconFromStyle(iconName));
        else
            this->setWindowIcon(QIcon::fromTheme(iconName));
    }
    this->setModal(true);
    this->setWindowTitle(title);
    this->centerWindow();
    this->setMinimumSize(SETT.getValue(SETTINGS_GUI_LOGIN_DIALOG_MIN_WIDTH).toInt(), SETT.getValue(SETTINGS_GUI_LOGIN_DIALOG_MIN_HEIGHT).toInt());

    m_VLayoutDialog = new CVBoxLayout(this);
    m_headerLayout.reset(new CGridLayout());
    m_bodyLayout.reset(new CFormLayout());
    m_footerLayout.reset(new CHBoxLayout());
    addFields();
    addFieldsAndComponents();

    connect(getButtonBox().get(), &CButtonBoxOk::accepted, this, &CLoginDialog::accept);
    connect(getButtonBox().get(), &CButtonBoxOk::rejected, this, &CLoginDialog::reject);

    this->restoreDialogState();
    m_passwd->setFocus();
}

CLoginDialog::~CLoginDialog()
{
    this->saveDialogState();
    DEBUG_WITH_LINE << "~dtor ";
}

void CLoginDialog::addFieldsAndComponents()
{
    m_headerLayout->addWidget(m_icon_label.get(), 0, 0, 1, 1);
    m_headerLayout->addWidget(m_version_label.get(), 0, 1, 1, 2);
    m_headerLayout->addWidget(m_company_label.get(), 1, 1, 1, 2);

    CForm::setComponents(m_bodyLayout.get(), m_passwd.get(), m_passwd->getLabel(), 0);

    //m_footerLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_footerLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_footerLayout->addWidget(m_buttonBox.get());

    m_VLayoutDialog->addLayout(m_headerLayout.get());
    m_VLayoutDialog->addWidget(new CHLine());
    m_VLayoutDialog->addLayout(m_bodyLayout.get());
    //m_VLayoutDialog->addWidget(QSharedPointer<CHLine>(new CHLine()).get());
    m_VLayoutDialog->addWidget(new CHLine());
    m_VLayoutDialog->addLayout(m_footerLayout.get());
}

void CLoginDialog::addFields()
{
//Fields
    m_passwd = QSharedPointer<CFormTextField>(new CFormTextField(QString("m_passwd"), tr("Hasło do bazy"), "", 32));
    m_passwd->setPasswdEcho();
//Przyciski
    m_buttonBox = QSharedPointer<CButtonBoxOk>(new CButtonBoxOk(this));
//Labele
    m_icon_label = QSharedPointer<CLabel>(new CLabel(QString(), ICON("WebPassWare")));
    m_version_label = QSharedPointer<CLabel>(new CLabel(tr("Wersja programu: %1").arg(CApplication::toString())));
    m_company_label = QSharedPointer<CLabel>(new CLabel(tr("Copyright © 2021 %1").arg(APP_ORGANIZATION_NAME)));
}

//Zapisanie i odtworzenie układu graficznego dialogu
void CLoginDialog::restoreDialogState()
{
    QByteArray state = getDialogState()->getState(QString("LOGIN_DIALOG"));
    this->restoreGeometry(state);
}

void CLoginDialog::saveDialogState()
{
    QByteArray state = this->saveGeometry();
    QByteArray oldState = getDialogState()->getState(QString("LOGIN_DIALOG"));

    if(state != oldState)
        getDialogState()->saveState(QString("LOGIN_DIALOG"), state);
}

void CLoginDialog::accept()
{
    //Passwd check;
    QString check_passwd =  QCryptographicHash::hash(this->getPasswd()->text().toLatin1(), QCryptographicHash::Md5).toHex();
    QString hash_passwd = DB.getDatabaseHashPassword();

    if (hash_passwd.isEmpty())
    {
        CMessageBox::OkDialogInformation(tr("Ustaw hasło do bazy danych.\nMenu->Ustawienia->Opcje programu->Daza danych"), this);
        CDialog::accept();
    }
    else if (check_passwd ==  hash_passwd)
    {
        CDialog::accept();
    }
    else
    {
        CMessageBox::OkDialogInformation(tr("Niepoprawne hasło !!!"), this);
    }
    return;
}
