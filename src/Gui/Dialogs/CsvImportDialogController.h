#ifndef CCSVIMPORTDIALOGCONTROLLER_H
#define CCSVIMPORTDIALOGCONTROLLER_H

#include "Global.h"
#include "CsvImportDialog.h"
#include "FileDialog.h"
#include "AbstractDialogStateManager.h"
#include "CsvModel.h"
#include "ImportCsv.h"

class CCsvImportDialogController : public QObject, public CAbstractDialogStateManager
{
    Q_OBJECT

    ADD_PTR_PRO(CCsvImportDialog, m_dialog, getDialog)
    ADD_PTR_PRO(CCsvModel, m_model, getModel)
    ADD_PTR_PRO(CImportCsv, m_import, getImport)

    QSettings *m_settings;

public:
    CCsvImportDialogController(QObject *parent = NULL);
    ~CCsvImportDialogController();

    virtual void restoreDialogState();
    virtual void saveDialogState();

    void refreshModel();

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
