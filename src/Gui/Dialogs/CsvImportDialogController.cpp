#include "CsvImportDialogController.h"
#include "Application.h"
#include "PassEntryService.h"
#include "MessageBox.h"

CCsvImportDialogController::CCsvImportDialogController(QWidget *parent) : QWidget(parent)
{
    m_model = NULL;
    m_import = NULL;

    m_dialog = new CCsvImportDialog();
    APPI->setAppInformation();
    m_settings = new QSettings();
    m_settings->beginGroup("ImportCsvData");

    connect(m_dialog->getButtonOk(), SIGNAL(clicked()), this, SLOT(onAccept()));
    connect(m_dialog->getButtonCancel(), SIGNAL(clicked()), this, SLOT(onReject()));
    connect(m_dialog->getButtonChoiceFile(), SIGNAL(clicked()), this, SLOT(onButtonChoiceFileCsv()));
    connect(m_dialog->getButtonBack(), SIGNAL(clicked()), this, SLOT(onButtonBack()));
    connect(m_dialog->getButtonNext(), SIGNAL(clicked()), this, SLOT(onButtonNext()));
//Połącznia do combobox

    connect(m_dialog->getFieldsSeparator(), SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onFieldsSeparatorChange(const QString &)));
    connect(m_dialog->getFileCodec(), SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onFileCodecChange(const QString &)));
    connect(m_dialog->getDigitSign(), SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onDigitSignChange(const QString &)));
    connect(m_dialog->getFromLine(), SIGNAL(valueChanged(double)), this, SLOT(onFromLineChange(double)));
    connect(m_dialog->getIsTitle(), SIGNAL(toggled(bool)), this, SLOT(onIsTitleChange(bool)));

    changePage(0);
    restoreDialogState();
}

CCsvImportDialogController::~CCsvImportDialogController()
{
    saveDialogState();
    m_settings->endGroup();
    safe_delete(m_settings)
    safe_delete(m_dialog)
}

//Zapisanie i odtworzenie układu graficznego dialogu
void CCsvImportDialogController::restoreDialogState()
{
    QByteArray state = m_dialogState->getState(QString("IMPORT_CSV_DIALOG"));
    m_dialog->restoreGeometry(state);
}

void CCsvImportDialogController::saveDialogState()
{
    QByteArray state = m_dialog->saveGeometry();
    QByteArray oldState = m_dialogState->getState(QString("IMPORT_CSV_DIALOG"));

    if(state != oldState)
        m_dialogState->saveState(QString("IMPORT_CSV_DIALOG"), state);
}

void CCsvImportDialogController::onButtonChoiceFileCsv()
{
    QString fileName("");
    QString fileType = tr("Plik CSV (*.csv)");

    fileName = CFileDialog::getOpenFileName(m_dialog, tr("Wybierz plik"), QDir::currentPath(), fileType);
    if (fileName.length() > 0)
        {
            m_dialog->getFileNameCsv()->setValue(fileName);
            //if (m_import)       delete importu jest w model`u
            //    delete m_import;
            if (m_model)
                delete m_model;
            m_import = new CImportCsv(fileName, QChar('\t'));

            m_model = new CCsvModel(m_import);
            m_dialog->getTableView()->setModel(m_model);
            m_dialog->getTableViewPreview()->setModel(m_model);
//Pobranie danych z formatki i ustawienie importu
            QString sep = m_dialog->getFieldsSeparator()->getValue().toString();
            QChar digitSign = m_dialog->getDigitSign()->getValue().toChar();
            int fromLine = m_dialog->getFromLine()->getValue().toInt();
            bool isTitle = m_dialog->getIsTitle()->getValue().toBool();
            QString codecName = m_dialog->getFileCodec()->getValue().toString();

            m_model->setFromLine(fromLine);
            m_import->setCodec(codecName);
            m_import->setDigitSign(digitSign);
            m_import->setIsTitle(isTitle);
            if (sep == QString("TAB"))
                m_import->setDelimeter(QChar('\t'));
            else
                m_import->setDelimeter(QChar(sep.toLatin1().data()[0]));

            this->refreshModel();
        }
}

void CCsvImportDialogController::onButtonNext()
{
    if (m_model)
    {
        m_model->setIsTitle(m_dialog->getIsTitle()->getValue().toBool());
        m_dialog->getTitleChoice()->setValueList(m_model->getColumnForImport());
        m_dialog->getUserChoice()->setValueList(m_model->getColumnForImport());
        m_dialog->getPassChoice()->setValueList(m_model->getColumnForImport());
        m_dialog->getWebChoice()->setValueList(m_model->getColumnForImport());
        m_dialog->getDescChoice()->setValueList(m_model->getColumnForImport());
        changePage(1);
    }
    else
        CMessageBox::OkDialogInformation(tr("Nie wybrano pliku do importu !!!"), this);
}

void CCsvImportDialogController::onButtonBack()
{
    changePage(0);
}

void CCsvImportDialogController::changePage(int idx)
{
    if (idx == 0)
    {
        m_dialog->getButtonOk()->hide();
        m_dialog->getButtonBack()->hide();
        m_dialog->getButtonNext()->show();

    }
    else if (idx == 1)
    {
        m_dialog->getButtonOk()->show();
        m_dialog->getButtonBack()->show();
        m_dialog->getButtonNext()->hide();
    }
    m_dialog->getStackedWidget()->setCurrentIndex(idx);
}

void CCsvImportDialogController::refreshModel()
{
    if (m_model)
        {
            m_model->refresh();
            m_dialog->getTableView()->resizeColumnsToContents();
            m_dialog->getTableViewPreview()->resizeColumnsToContents();
            m_dialog->getFromLine()->setMaximum(m_model->getTableSize());
        }
}

void CCsvImportDialogController::onFieldsSeparatorChange(const QString &sep)
{
    if (m_import)
        {
            if (sep == QString("TAB"))
                m_import->setDelimeter(QChar('\t'));
            else
                m_import->setDelimeter(QChar(sep.toLatin1().data()[0]));
        }
    refreshModel();
}

void CCsvImportDialogController::onIsTitleChange(bool checked)
{
    if (m_import)
        m_import->setIsTitle(checked);
    refreshModel();
}

void CCsvImportDialogController::onFileCodecChange(const QString &codecName)
{
    if (m_import)
        m_import->setCodec(codecName);
    refreshModel();
}

void CCsvImportDialogController::onDigitSignChange(const QString &digitSign)
{
    if (m_import)
        m_import->setDigitSign(digitSign.toLatin1().data()[0]);
    refreshModel();
}

void CCsvImportDialogController::onFromLineChange(double nr)
{
    int nr_line = static_cast<int>(nr);
    if (m_model)
        m_model->setFromLine(nr_line);
    refreshModel();
}

void CCsvImportDialogController::onAccept()
{
    QString filename = m_dialog->getFileNameCsv()->getValue().toString();

    if (filename.length() > 0)
       {        
        if (m_model)
           {
               for (int r = 0; r < m_model->rowCount(); r++)
               {
                  PassEntry pe;
                  pe.setm_title(m_model->getValueForImport(r, m_dialog->getTitleChoice()->getValue().toString()).toString());
                  pe.setm_user(m_model->getValueForImport(r, m_dialog->getUserChoice()->getValue().toString()).toString());
                  pe.setm_pass(m_model->getValueForImport(r, m_dialog->getPassChoice()->getValue().toString()).toString());
                  pe.setm_web_url(m_model->getValueForImport(r, m_dialog->getWebChoice()->getValue().toString()).toString());
                  pe.setm_desc(m_model->getValueForImport(r, m_dialog->getDescChoice()->getValue().toString()).toString());
                  PassEntryService::getInstance().addObject(&pe);
                }
               m_dialog->accept();
           }
        else
           {
                CMessageBox::OkDialogInformation(tr("Brak modelu danych do importu !!!"), this);
           }
       }
    else
       {
            CMessageBox::OkDialogInformation(tr("Nie wybrano pliku do importu !!!"), this);
       }
}

void CCsvImportDialogController::onReject()
{
    m_dialog->reject();
}

