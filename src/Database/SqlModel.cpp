#include "SqlModel.h"
#include "BusyIndicator.h"
#include "Database.h"

CSqlModel::CSqlModel(CDatabase &db, const QString &selectQuery, const QString &table_name, const SqlFieldsDesc &list, QVariantHash data, QObject *parent) :
    QSqlQueryModel(parent), m_db(db)
{
    initSqlModel();
    m_selectQuery = selectQuery;
    m_tableName = table_name;
    m_fieldsDesc = list;
    m_dataHash = data;
    m_where.clear();
}

void CSqlModel::initSqlModel()
{
    m_hasDeleteDateField = false;
    m_primaryKeyIdx = 0;
    // Was left uninitialised: data() then hid or centred values at random.
    m_hideNulls = false;
    m_sortKeyColumn = -1;
    m_sortOrder = Qt::AscendingOrder;
}

QVariant CSqlModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
      {
        QVariant var = QSqlQueryModel::data(index, role);

        if (m_hideNulls && var.isNull())
          return QString("-");
        //Pobranie danych z opisu rekordu
        QString column_name = this->record().fieldName(index.column());
        if (m_fieldsDesc.size() > 0 && m_fieldsDesc.value(column_name))
           {
             auto field_desc = m_fieldsDesc.value(column_name);
             if (field_desc && field_desc->isNumericField())
                {
                 int precision = field_desc->getPrecision();
                 return QString::number(var.toDouble(), 'f', precision).toDouble();
                }
           }
        //Pobranie danych z kolumny numerycznej
        if (m_numericColumns.indexOf(index.column()) != -1)
            {
                return QString::number(var.toDouble(), 'f', 2).toDouble();
            }
        return var;
      }
    else if (role == Qt::TextAlignmentRole)
          {
            if  (m_hideNulls)
                {
                    QVariant var = QSqlQueryModel::data(index, role);
                    if (var.isNull())
                        return Qt::AlignCenter;
                }
            int type = QSqlQueryModel::data(index, role).userType();
            QString column_name = this->record().fieldName(index.column());

            if (m_fieldsDesc.size() > 0 && m_fieldsDesc.value(column_name))
                type = m_fieldsDesc.value(column_name)->getType();
            if (type == QMetaType::Int || type == QMetaType::Double || m_numericColumns.indexOf(index.column()) != -1)
              return int(Qt::AlignRight | Qt::AlignVCenter);
            else
              return int(Qt::AlignLeft | Qt::AlignVCenter);
          }
    return QVariant();
}

//Ustawienie nazw kolumn w modelu do wyświetlania
QVariant CSqlModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
      if (role == Qt::DisplayRole)
        {
            QString column_name = this->record().fieldName(section);
            if (m_fieldsDesc.size() > 0 && m_fieldsDesc.value(column_name))
                column_name = m_fieldsDesc.value(column_name)->getName();
            return column_name;
         }
      if (role == Qt::ToolTipRole)
         {
            QString column_name = this->record().fieldName(section);
            if (m_fieldsDesc.size() > 0 && m_fieldsDesc.value(column_name))
                return m_fieldsDesc.value(column_name)->getDescription();
         }
    }
    return QSqlQueryModel::headerData(section, orientation, role);
}

QVariant CSqlModel::rawData(const QModelIndex &index, int role) const
{
    return QSqlQueryModel::data(index,role);
}

void CSqlModel::refresh()
{
      wpw::BusyIndicator::Scope busy;
        {
          if (m_db.isConnected())
             {
                CSqlQuery query(m_db.getDb());
                query.setLogFromDB(m_db.getEchoError(), m_db.getLogError(), m_db.getEchoError(), m_db.getLogQuery());
                query.setForwardOnly(false);

                QString select_query = m_selectQuery;                
                select_query += QString(" WHERE deleted=false ");

                if (!m_where.isEmpty())
                    select_query += QString(" AND %1 " ).arg(m_where);

                query.prepare(select_query);
                for (auto variable : m_dataHash.keys())
                    {
                        QVariant value = m_dataHash.value(variable);
                        query.bindValue(variable, value);
                    }
                DEBUG_WITH_LINE << query.lastQuery();

//                beginResetModel();  //Uwaga bardzo ważne. bez tego nie wywołuje się data i headerData
                // The executed query itself, so the bound values are used.
                query.exec();
                QSqlQueryModel::setQuery(std::move(query));
                if (this->lastError().isValid())
                    DEBUG_WITH_LINE <<  this->lastError();
                else
                {
                    DEBUG_WITH_LINE << "Model OK";
                    emit querySuccess();
                }
//                endResetModel();
             }
          else if (this->lastError().isValid())
                   DEBUG_WITH_LINE <<  this->lastError();
        }
}

QString CSqlModel::getFieldNameByColumn(int column)
{
    for (auto field : std::as_const(m_fieldsDesc))
    {
        if (field->getPos() == column)
            return m_fieldsDesc.key(field);
    }
    return QString();
}

int CSqlModel::columnIndex(const QString &columnName)
{
      return this->record().indexOf(columnName);
}

void CSqlModel::setSort(int column, Qt::SortOrder order)
{
    DEBUG_WITH_LINE << "kolumna do sortowania: " << column;

    m_sortKeyColumn = column;
    m_fieldSortName = getFieldNameByColumn(column);
    m_sortOrder = order;
    DEBUG_WITH_LINE << "Kolejnosc sortowania: " << m_sortOrder;
}

QString CSqlModel::getSelectQueryStringFromFieldsDescList(SqlFieldsDesc fieldsDesc, const QString &from)
{
    QString query = QString("SELECT "), column;
    QList<CSqlField*> list = fieldsDesc.values(); //Lista pól
   //Posortowna lista po pozycji
    std::sort(list.begin(), list.end(), CSqlField::positionLessThan);
    for (auto f : std::as_const(list))
        {
            column = fieldsDesc.key(f);
            if (f->getSqlDescription().isEmpty())
                query += column;
            else
                {
                  query += QString("(");
                  query += f->getSqlDescription();
                  query += QString(") ");
                  query += QString(" AS ");
                  if (f->getFlags() & FIELD_SELECT_AS_QUOTE)
                      query += "'" + column + "'";
                  else
                      query += column;
                }
            query += QString(", ");
        }
    query.chop(2);
    if (!from.isEmpty())
    {
        query += QString(" FROM ");
        query += from;
    }
    return query;
}

QList< QVector<QVariant> > CSqlModel::toVariantRowListData()
{
    QList< QVector<QVariant> > list;
    QVector <QVariant> row;
    QModelIndex idx;

    list.clear();
    for (int r = 0 ; r <  this->rowCount() ; r++)
    {
        row.clear();
        for (int c = 0 ; c < this->columnCount() ; c++)
        {
            idx = this->index(r, c);
            if (idx.isValid())
            {
                row.append(this->data(idx, Qt::DisplayRole));
            }
        }
        list.append(row);
    }
    return list;
}

QVector<QVariant> CSqlModel::toVariantRowHeader()
{
    QVector <QVariant> row;

    row.clear();
    for (int c = 0 ; c < this->columnCount() ; c++)
    {
        row.append(this->headerData(c,  Qt::Horizontal, Qt::DisplayRole));
    }
    return row;
}
