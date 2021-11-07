#include "DataExportDialog.h"

CDataExportDialog::CDataExportDialog(QWidget *parent) : CDialog(parent)
{

    this->setWindowTitle(tr("Eksport danych"));
    this->centerWindow();
    addFields();

    m_VLayoutDialog = new CVBoxLayout(this);
    m_buttonBox = new CButtonBoxExport(this);
    m_formLayout_file_csv  = new CGridLayout();

    //Przyciski
    addButtons();
    //Pola
    addComponents();
    m_VLayoutDialog->addLayout(m_formLayout_file_csv);
    m_VLayoutDialog->addWidget(m_buttonBox);
}

void CDataExportDialog::addField(CFormAbstractField *f)
{
   m_fields.insert(f->getVariableName(), f);
}

void CDataExportDialog::addFields()
{
    m_fileNameCsv = new CFormTextField(QString("m_fileNameCsv"), tr("Nazwa pliku"), "", 255);
    addField(m_fileNameCsv);
    m_digitSign = new CFormSimpleChoiceField(QString("m_digitSign"), tr("Wybierz znak cyfry po przecinku"), 0, getDigitSigns());
    addField(m_digitSign);
    m_fieldsSeparator = new CFormSimpleChoiceField(QString("m_fieldsSeparator"), tr("Wybierz separator pól"), 0, getFieldsSeparators());
    addField(m_fieldsSeparator);
    m_fileCodec = new CFormSimpleChoiceField(QString("m_fileCodec"), tr("Wybierz kodowanie polskich znaków"), 0, getFileCodecs());
    addField(m_fileCodec);
}

void CDataExportDialog::addComponents()
{
    CForm::setComponents(m_formLayout_file_csv, m_fileNameCsv, m_fileNameCsv->getLabel(), 1, 1, 1, 2);
    //Przycisk wyboru pliku
    CForm::setComponents(m_formLayout_file_csv, m_buttonChioceFileCsv, QString(""), 1, 3, 1, 1);

    CForm::setComponents(m_formLayout_file_csv, m_fieldsSeparator, m_fieldsSeparator->getLabel(), 2, 1, 1, 1);
    CForm::setComponents(m_formLayout_file_csv, m_fileCodec, m_fileCodec->getLabel(), 3, 1, 1, 2);
    CForm::setComponents(m_formLayout_file_csv, m_digitSign, m_digitSign->getLabel(), 4, 1, 1, 1);

}

void CDataExportDialog::addButtons()
{
    m_buttonChioceFileCsv = new CButton(CButtonPrivate(tr("Wybierz plik ..."), tr("Wybierz plik do exportu"), ICON("File")));
}

QStringList CDataExportDialog::getDigitSigns(void)
{
    return QStringList() << QString(",") << QString(".");
}

QStringList CDataExportDialog::getFileCodecs(void)
{
  return QStringList() << QString("ISO-8859-2") << QString("Windows-1250") << QString("UTF-8") << QString("DOS/OS2-852");
/*  QStringList l;

    QList<QByteArray> list =  QTextCodec::availableCodecs();
    Q_FOREACH (auto codec, list)
        {
            l.append(QString(codec));
        }
    return l;
*/
}
QStringList CDataExportDialog::getFieldsSeparators(void)
{
    return QStringList() << QString("TAB") << QString("|") << QString(";") << QString(",");
}
