#include "DataExportDialogController.h"
#include "MessageBox.h"
#include "ExportCsv.h"
#include "Application.h"

CDataExportDialogController::CDataExportDialogController(CSqlModel *model, QWidget *parent) :
    QWidget(parent), m_model(model)
{
    m_dialog = new CDataExportDialog();
    APPI->setAppInformation();
    m_settings = new QSettings();
    m_settings->beginGroup("ExportData");
    restoreLastState();
    restoreDialogState();

    connect(m_dialog->getButtonBox(), SIGNAL(accepted()), this, SLOT(onAccept()));
    connect(m_dialog->getButtonBox(), SIGNAL(rejected()), this, SLOT(onReject()));
    connect(m_dialog->getButtonChoiceFileCsv(), SIGNAL(clicked()), this, SLOT(onButtonChoiceFileCsv()));
}

CDataExportDialogController::~CDataExportDialogController()
{
    saveLastState();
    saveDialogState();
    m_settings->endGroup();
    safe_delete(m_settings)
    safe_delete(m_dialog)
}

void CDataExportDialogController::saveLastState()
{
    m_settings->setValue(m_dialog->getFileNameCsv()->getVariableName(), m_dialog->getFileNameCsv()->getValue().toString());
    m_settings->setValue(m_dialog->getFieldsSeparator()->getVariableName(), m_dialog->getFieldsSeparator()->getValue().toString());
    m_settings->setValue(m_dialog->getDigitSign()->getVariableName(), m_dialog->getDigitSign()->getValue().toString());
    m_settings->setValue(m_dialog->getFileCodec()->getVariableName(), m_dialog->getFileCodec()->getValue().toString());
}

void CDataExportDialogController::restoreLastState()
{
    m_dialog->getFileNameCsv()->setValue(m_settings->value(m_dialog->getFileNameCsv()->getVariableName()));
    m_dialog->getFileNameCsv()->setStartValue(m_settings->value(m_dialog->getFileNameCsv()->getVariableName()));
    m_dialog->getFieldsSeparator()->setValue(m_settings->value(m_dialog->getFieldsSeparator()->getVariableName()));
    m_dialog->getFieldsSeparator()->setStartValue(m_settings->value(m_dialog->getFieldsSeparator()->getVariableName()));
    m_dialog->getDigitSign()->setValue(m_settings->value(m_dialog->getDigitSign()->getVariableName()));
    m_dialog->getDigitSign()->setStartValue(m_settings->value(m_dialog->getDigitSign()->getVariableName()));
    m_dialog->getFileCodec()->setValue(m_settings->value(m_dialog->getFileCodec()->getVariableName()));
    m_dialog->getFileCodec()->setStartValue(m_settings->value(m_dialog->getFileCodec()->getVariableName()));
}

//Zapisanie i odtworzenie układu graficznego dialogu
void CDataExportDialogController::restoreDialogState()
{
    QByteArray state = m_dialogState->getState(QString("EXPORT_DIALOG"));
    m_dialog->restoreGeometry(state);
}

void CDataExportDialogController::saveDialogState()
{
    QByteArray state = m_dialog->saveGeometry();
    QByteArray oldState = m_dialogState->getState(QString("EXPORT_DIALOG"));

    if(state != oldState)
        m_dialogState->saveState(QString("EXPORT_DIALOG"), state);
}

bool CDataExportDialogController::wrtite2Csv(const QString &fileName, const QChar &delimeter, const QString &codecName, const QChar &digitSign)
{
    CExportCsv *eCsv = new CExportCsv(fileName, delimeter, codecName, digitSign);

    if (!eCsv->getFile()->isWritable())
       {
         delete eCsv;
         return false;
       }
    if (m_model)
       {
         QVector<QVariant> header = m_model->toVariantRowHeader();
         QList< QVector<QVariant> > data =  m_model->toVariantRowListData();
         eCsv->writeLine(header);
         Q_FOREACH(auto row, data)
         {
             eCsv->writeLine(row);
         }
       }
    delete eCsv;
    return true;
}

void CDataExportDialogController::onButtonChoiceFileCsv()
{
    QString fileName("");
    QString fileType(tr("Plik CSV (*.csv)"));

    fileName = CFileDialog::getSaveFileName(m_dialog, tr("Wybierz plik"), QDir::currentPath(), fileType, QFileDialog::DontConfirmOverwrite, true, m_dialog->getFileNameCsv()->getValue().toString());
    if (!fileName.isEmpty())
        {
           if (!fileName.contains(".csv") && !fileName.contains(".CSV"))
               fileName.append(".csv");
           m_dialog->getFileNameCsv()->setValue(fileName);
        }
}

void CDataExportDialogController::onAccept()
{
    QString filename = m_dialog->getFileNameCsv()->getValue().toString();

    if (filename.length() > 0)
       {
        if (!filename.contains(".csv") && !filename.contains(".CSV"))
            filename.append(".csv");
        m_dialog->getFileNameCsv()->setValue(filename);

        QChar delimeter = m_dialog->getFieldsSeparator()->getValue().toString().at(0).toLatin1();
        QString codecName = m_dialog->getFileCodec()->getValue().toString();
        QChar digitSign = m_dialog->getDigitSign()->getValue().toString().at(0).toLatin1();
        if (m_dialog->getFieldsSeparator()->getValue().toString() == QString("TAB"))
            delimeter = QChar('\t');

        if (wrtite2Csv(filename, delimeter, codecName, digitSign))
            CMessageBox::OkDialogInformation(tr("Dane zostały pomyślnie zapisane."), m_dialog);
         else
            CMessageBox::OkDialogCritical(tr("Dane nie zostały pomyślnie zapisane !!!"), m_dialog);

        m_dialog->accept();
       }
}

void CDataExportDialogController::onReject()
{
    m_dialog->reject();
}
