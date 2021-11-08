#include "CsvImportDialog.h"

CCsvImportDialog::CCsvImportDialog(QWidget *parent) : CDialog(parent)
{
    this->setWindowTitle(tr("Import danych z CSV"));
    this->centerWindow();    
    addFields();
    m_VLayoutDialog = new CVBoxLayout(this);
    m_stackedWidget = new QStackedWidget(this);
    m_widget_first = new QWidget();
    m_widget_second = new QWidget();

    m_stackedWidget->addWidget(m_widget_first);
    m_stackedWidget->addWidget(m_widget_second);

    m_formLayout_first  = new CGridLayout(m_widget_first);
    m_formLayout_second  = new CGridLayout(m_widget_second);
    m_buttonLayout  = new CHBoxLayout();
    //Przyciski
    addButtons();
    //Pola
    addComponents();
    m_VLayoutDialog->addWidget(m_stackedWidget);
    m_VLayoutDialog->addLayout(m_buttonLayout);
}

void CCsvImportDialog::addField(CFormAbstractField *f)
{
   m_fields.insert(f->getVariableName(), f);
}


void CCsvImportDialog::addFields()
{
//First page
    m_fileNameCsv = new CFormTextField(QString("m_fileNameCsv"), tr("Nazwa pliku"), "", 255, true);
    addField(m_fileNameCsv);
    m_isTitle = new CFormCheckField(QString("m_isTitle"), tr("Czy jest tytuł ?"), true);
    addField(m_isTitle);
    m_digitSign = new CFormSimpleChoiceField(QString("m_digitSign"), tr("Wybierz znak cyfry po przecinku"), 0, getDigitSigns());
    addField(m_digitSign);
    m_fieldsSeparator = new CFormSimpleChoiceField(QString("m_fieldsSeparator"), tr("Wybierz separator pól"), 0, getFieldsSeparators());
    addField(m_fieldsSeparator);
    m_fileCodec = new CFormSimpleChoiceField(QString("m_fileCodec"), tr("Wybierz kodowanie polskich znaków"), 0, getFileCodecs());
    addField(m_fileCodec);
    m_fromLine = new CFormNumberField(QString("m_fromLine"), tr("Od linii"), 0);
    addField(m_fromLine);
    m_tableViewPreview = new CTableView();
//Second page
    m_titleChoice = new CFormSimpleChoiceField(QString("m_titleChoice"), tr("Wybierz kolumnę dla tytułu"), 0);
    addField(m_titleChoice);
    m_userChoice = new CFormSimpleChoiceField(QString("m_userChoice"), tr("Wybierz kolumnę dla użytkownika"), 0);
    addField(m_userChoice);
    m_passChoice = new CFormSimpleChoiceField(QString("m_passChoice"), tr("Wybierz kolumnę dla hasła"), 0);
    addField(m_passChoice);
    m_webChoice = new CFormSimpleChoiceField(QString("m_webChoice"), tr("Wybierz kolumnę dla strony www"), 0);
    addField(m_webChoice);
    m_descChoice = new CFormSimpleChoiceField(QString("m_descChoice"), tr("Wybierz kolumnę dla opisu"), 0);
    addField(m_descChoice);
    m_tableView = new CTableView();
}

void CCsvImportDialog::addButtons()
{
    m_buttonChioceFile = new CButton(CButtonPrivate(tr("Wybierz plik ..."), tr("Wybierz plik do importu"), ICON("Open")));
    m_buttonOk = new CButton(CButtonPrivate(tr("&Ok"), tr("Ok"), ICON("Ok")));
    m_buttonCancel = new CButton(CButtonPrivate(tr("&Anuluj"), tr("Anuluj"), ICON("Cancel")));
    m_buttonBack = new CButton(CButtonPrivate(tr("&Wstecz"), tr("Wstecz"), ICON("Back")));
    m_buttonNext = new CButton(CButtonPrivate(tr("&Dalej"), tr("Dalej"), ICON("Forward")));

    m_buttonLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_buttonLayout->addWidget(m_buttonBack);
    m_buttonLayout->addWidget(m_buttonNext);
    m_buttonLayout->addWidget(m_buttonCancel);
    m_buttonLayout->addWidget(m_buttonOk);
}

void CCsvImportDialog::addComponents()
{
//First page
    CForm::setComponents(m_formLayout_first, m_fileNameCsv, m_fileNameCsv->getLabel(), 1, 1, 1, 2);
    CForm::setComponents(m_formLayout_first, m_buttonChioceFile, QString(""), 1, 3, 1, 1);
    CForm::setComponents(m_formLayout_first, m_fieldsSeparator, m_fieldsSeparator->getLabel(), 2, 1, 1, 1);
    CForm::setComponents(m_formLayout_first, m_fileCodec, m_fileCodec->getLabel(), 3, 1, 1, 1);
    CForm::setComponents(m_formLayout_first, m_digitSign, m_digitSign->getLabel(), 4, 1, 1, 1);
    CForm::setComponents(m_formLayout_first, m_isTitle, m_isTitle->getLabel(), 5, 1, 1, 1);
    CForm::setComponents(m_formLayout_first, m_fromLine, m_fromLine->getLabel(), 6, 1, 1, 1);
    m_formLayout_first->addWidget(m_tableViewPreview, 7, 0, 4, 4);
//Second page
    CForm::setComponents(m_formLayout_second, m_titleChoice, m_titleChoice->getLabel(), 1, 1, 1, 1);
    CForm::setComponents(m_formLayout_second, m_userChoice, m_userChoice->getLabel(), 2, 1, 1, 1);
    CForm::setComponents(m_formLayout_second, m_passChoice, m_passChoice->getLabel(), 3, 1, 1, 1);
    CForm::setComponents(m_formLayout_second, m_webChoice, m_webChoice->getLabel(), 4, 1, 1, 1);
    CForm::setComponents(m_formLayout_second, m_descChoice, m_descChoice->getLabel(), 5, 1, 1, 1);
    m_formLayout_second->addWidget(m_tableView, 6, 0, 4, 4);
}

QStringList CCsvImportDialog::getDigitSigns()
{
    return QStringList() << QString(",") << QString(".");
}

QStringList CCsvImportDialog::getFileCodecs()
{
  return QStringList() << QString("ISO-8859-2") << QString("Windows-1250") << QString("UTF-8") << QString("DOS/OS2-852");
//  QStringList l;

//    QList<QByteArray> list =  QTextCodec::availableCodecs();
//    Q_FOREACH (auto codec, list)
//        {
//            l.append(QString(codec));
//        }
//    return l;
}

QStringList CCsvImportDialog::getFieldsSeparators()
{
    return QStringList() << QString("TAB") << QString("|") << QString(";") << QString(",");
}
