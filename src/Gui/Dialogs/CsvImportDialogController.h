#ifndef CCSVIMPORTDIALOGCONTROLLER_H
#define CCSVIMPORTDIALOGCONTROLLER_H

#include "Global.h"
#include "CsvImportDialog.h"
#include "FileDialog.h"
#include "AbstractDialogStateManager.h"
#include "CsvModel.h"
#include "ImportCsv.h"

class PassEntryService;

class CCsvImportDialogController : public QWidget, public CAbstractDialogStateManager
{
    Q_OBJECT

    ADD_QSMART_SHARED_PTR(CCsvImportDialog, m_dialog, getDialog)
    ADD_QSMART_SHARED_PTR(CCsvModel, m_model, getModel)
    ADD_QSMART_SHARED_PTR(CImportCsv, m_import, getImport)
    ADD_QSMART_UNIQUE_PTR(QSettings, m_settings, getDataImportSettings)

public:
    CCsvImportDialogController(QWidget *parent = nullptr);
    CCsvImportDialogController(PassEntryService *passEntryService, QWidget *parent = nullptr);
    ~CCsvImportDialogController();

    virtual void restoreDialogState() override final;
    virtual void saveDialogState() override final;

    void refreshModel();

private:
    void saveLastState();
    void restoreLastState();

    PassEntryService *m_passEntryService;

public slots:

  void onButtonChoiceFileCsv();
  void onButtonNext();
  void onButtonBack();

  void onFieldsSeparatorChange(const QString &);
  void onIsTitleChange(bool);
  void onFileCodecChange(const QString &);
  void onDigitSignChange(const QString &);
  void onFromLineChange(double);

  void changePage(int idx);

  void onAccept();
  void onReject();

};

#endif // CCSVIMPORTDIALOGCONTROLLER_H
