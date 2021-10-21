#include <QApplication>
#include "SqlModel.h"
#include "Database.h"

CSqlModel::CSqlModel(const QString &selectQuery, const QString &table_name, const SqlFieldsDesc &list, QVariantHash data, QObject *parent) : QSqlQueryModel(parent)
{
    initSqlModel();
    m_selectQuery = selectQuery;
    m_tableName = table_name;
    m_fieldsDesc = list;
    m_dataHash = data;
}

void CSqlModel::initSqlModel()
{
    m_hasDeleteDateField = false;
    m_primaryKeyIdx = 0;
}

QVariant CSqlModel::data(const QModelIndex &index, int role) const
{

    if (role == Qt::DisplayRole)
      {
        QVariant var = this->data(index, role);

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
                    QVariant var = this->data(index,role);
                    if (var.isNull())
                        return Qt::AlignCenter;
                }
            QVariant type = this->data(index,role).type();
            QString column_name = this->record().fieldName(index.column());

            if (m_fieldsDesc.size() > 0 && m_fieldsDesc.value(column_name))
                type = m_fieldsDesc.value(column_name)->getType();
            if (type == QVariant::Int || type == QVariant::Double || m_numericColumns.indexOf(index.column()) != -1)
              return Qt::AlignRight + Qt::AlignVCenter;
            else
              return Qt::AlignLeft + Qt::AlignVCenter;
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
    //return QAbstractItemModel::headerData(section, orientation, role);
    return this->headerData(section, orientation, role);
}

void CSqlModel::refresh()
{
      if (qApp)
          qApp->setOverrideCursor(QCursor(Qt::BusyCursor));
        {
          if (DB.isConnected())
             {
                CSqlQuery query(DB.getDb());
                query.setLogFromDB(DB.getEchoError(),DB.getLogError(),DB.getEchoError(), DB.getLogQuery());
                query.setForwardOnly(false);

                QString select_query = m_selectQuery;

                if (m_selectQuery.contains("WHERE"))
                    select_query += QString(" AND deleted=false ");
                else
                    select_query += QString(" WHERE deleted=false ");

                query.prepare(select_query);
                Q_FOREACH (auto variable, m_dataHash.keys())
                    {
                        QVariant value = m_dataHash.value(variable);
                        query.bindValue(variable, value);
                    }
                DEBUG_WITH_LINE << query.lastQuery();

                //beginResetModel();  //Uwaga bardzo ważne. bez tego nie wywołuje się data i headerData
                this->setQuery(query.lastQuery(), DB.getDb());
                if (this->lastError().isValid())
                    DEBUG_WITH_LINE <<  this->lastError();
                else
                {
                    DEBUG_WITH_LINE << "Model OK";
                    emit querySuccess();
                }
                //endResetModel();
             }
          else if (this->lastError().isValid())
                   DEBUG_WITH_LINE <<  this->lastError();
        }
      if (qApp)
          qApp->restoreOverrideCursor();
}

QString CSqlModel::getFieldNameByColumn(int column)
{
    Q_FOREACH(auto field, m_fieldsDesc)
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
    Q_FOREACH(auto f, list)
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

