#ifndef CCsvExportDialogCONTROLLER_H
#define CCsvExportDialogCONTROLLER_H

#include "Global.h"
#include "CsvExportDialog.h"
#include "FileDialog.h"
#include "AbstractDialogStateManager.h"
#include "SqlModel.h"

class CCsvExportDialogController : public QWidget, public CAbstractDialogStateManager
{
    Q_OBJECT

    ADD_QSMART_SHARED_PTR(CCsvExportDialog, m_dialog, getDialog)
    ADD_QSMART_UNIQUE_PTR(QSettings, m_settings, getDataExportSettings)

public:
    CCsvExportDialogController(CSqlModel *model, QWidget *parent = NULL);
    ~CCsvExportDialogController();

    virtual void restoreDialogState() override final;
    virtual void saveDialogState() override final;


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

#endif // CCsvExportDialogCONTROLLER_H
