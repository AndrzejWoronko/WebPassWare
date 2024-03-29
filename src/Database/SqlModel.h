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
    ADD_FIELD(QString, m_where, getWhere, setWhere) //Dodatkowe where w query
    ADD_FIELD(QString, m_selectQuery, getSelectQuery, setSelectQuery) //select query
    void initSqlModel();

public:
    CSqlModel(const QString &selectQuery, const QString &table_name = QString() , const SqlFieldsDesc &list = SqlFieldsDesc(), QVariantHash data = QVariantHash(), QObject *parent = NULL);
    ~CSqlModel() { DEBUG_WITH_LINE << "~dtor ";}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant rawData(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void refresh();

    int columnIndex(const QString &columnName);
    void setSort (int column, Qt::SortOrder order);

    QString getFieldNameByColumn(int column);

    static QString getSelectQueryStringFromFieldsDescList(SqlFieldsDesc fieldsDesc, const QString &from); // opis pól

    QList< QVector<QVariant> > toVariantRowListData();
    QVector<QVariant> toVariantRowHeader();
private:

    int m_primaryKeyIdx;
    bool m_hasDeleteDateField;
    bool m_hideNulls;

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
