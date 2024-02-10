#ifndef CCSVMODEL_H
#define CCSVMODEL_H

#include "Global.h"
#include "ImportCsv.h"

using IQAbstractTableModel = QAbstractTableModel;

class CCsvModel : public IQAbstractTableModel
{
    ADD_FIELD(QString, m_fileName, getFileName, setFileName)
    ADD_FIELD(int, m_columns, getColumns, setColumns)
    ADD_FIELD(int, m_fromLine, getFormLine, setFromLine)
    ADD_FIELD(bool, m_isTitle, getIsTitle, setIsTitle)
    ADD_QSMART_SHARED_PTR(CImportCsv, m_importCsv, getImportCsv)

  public:
    CCsvModel(const QString &fileName, QObject *parent = NULL);
      CCsvModel(QSharedPointer<CImportCsv> impotCsv, QObject *parent = NULL);
    CCsvModel(const QString &fileName, const QChar &delimeter = QChar(';'), const QString &codecName = QString("iso8859-2"), const QChar & digitSign = QChar('.'), bool isTitle = true, QObject *parent = NULL);
    ~CCsvModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    void refresh();

    QStringList getHeaders();    
    QStringList getColumnForImport();
    int getTableSize();

    QVariant getValueForImport(int row, const QString &column);

  private:

    QList<QStringList> m_table;
    QStringList m_title;
    void readFile();
};

#endif // CCSVMODEL_H
