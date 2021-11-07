#ifndef CDATAEXPORTDIALOGCONTROLLER_H
#define CDATAEXPORTDIALOGCONTROLLER_H

#include "Global.h"
#include "DataExportDialog.h"
#include "FileDialog.h"
#include "AbstractDialogStateManager.h"
#include "SqlModel.h"

class CDataExportDialogController : public QWidget, public CAbstractDialogStateManager
{
    Q_OBJECT

    ADD_PTR(CDataExportDialog, m_dialog, getDialog)
    ADD_PTR(QSettings, m_settings, getDataExportSettings)

public:
    CDataExportDialogController(CSqlModel *model, QWidget *parent = NULL);
    ~CDataExportDialogController();

    virtual void restoreDialogState();
    virtual void saveDialogState();


private:

    bool wrtite2Csv(const QString &fileName, const QChar &delimeter, const QString &codecName, const QChar &digitSign);
    void saveLastState();
    void restoreLastState();

    CSqlModel* m_model;

public slots:

    void onButtonChoiceFileCsv();

    void onAccept();
    void onReject();
};

#endif // CDATAEXPORTDIALOGCONTROLLER_H
