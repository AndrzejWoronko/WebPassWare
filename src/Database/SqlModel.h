#ifndef CSQLMODEL_H
#define CSQLMODEL_H

#include "Global.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "SqlField.h"

class CSqlModel : public QSqlQueryModel
{    
    Q_OBJECT

    ADD_FIELD(QString, m_tableName, getTableName, setTableName)

    void initSqlModel();

public:
    CSqlModel(const QString &selectQuery, const QString &table_name = QString() , const SqlFieldsDesc &list = SqlFieldsDesc(), QVariantHash data = QVariantHash(), QObject *parent = NULL);
    ~CSqlModel() {};

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh();

    int columnIndex(const QString &columnName);
    void setSort (int column, Qt::SortOrder order);

    QString getFieldNameByColumn(int column);

    static QString getSelectQueryStringFromFieldsDescList(SqlFieldsDesc fieldsDesc, const QString &from); // opis pól

private:

    int m_primaryKeyIdx;
    bool m_hasDeleteDateField;
    bool m_hideNulls;

    QString m_selectQuery; //select query

    SqlFieldsDesc m_fieldsDesc; // opis pól
    QList<int> m_numericColumns;
    QVariantHash m_dataHash;

    int m_sortKeyColumn;
    Qt::SortOrder m_sortOrder;
    QString m_fieldSortName;

signals:

  void querySuccess();
};

#endif // CSQLMODEL_H
