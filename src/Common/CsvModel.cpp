#include "CsvModel.h"

CCsvModel::CCsvModel(const QString &fileName, QObject *parent) :
    QAbstractTableModel(parent), m_fileName(fileName), m_columns(0), m_fromLine(0)
{
    m_importCsv = new CImportCsv(fileName);
    //readFile();
}

CCsvModel::CCsvModel(const QString &fileName, const QChar &delimeter, const QString &codecName, const QChar & digitSign, bool isTitle, QObject *parent) :
    QAbstractTableModel(parent), m_fileName(fileName), m_columns(0), m_fromLine(0)
{
    m_isTitle = isTitle;
    m_importCsv = new CImportCsv(fileName, delimeter, codecName, digitSign, isTitle);
    //readFile();
}

CCsvModel::CCsvModel(CImportCsv *importCsv, QObject *parent) :
    QAbstractTableModel(parent),  m_columns(0), m_fromLine(0)
{
   m_importCsv = importCsv;
   m_fileName = importCsv->getFileName();
   //readFile();
}

CCsvModel::~CCsvModel()
{
     safe_delete(m_importCsv)
}

QVariant CCsvModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole && m_table.count() > index.row() + m_fromLine)
  {
    if (m_table[index.row() + m_fromLine].count() > index.column())
        return m_table[index.row() + m_fromLine][index.column()];
    else
        return QString("");
  }
  return QVariant();
}

QVariant CCsvModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole && m_importCsv->getIsTitle())
  {
    if (section < m_title.count())
      return m_title[section];
  }
  return QAbstractTableModel::headerData(section, orientation, role);
}


int CCsvModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return m_columns;
}

int CCsvModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return m_table.count() - m_fromLine;
}

void CCsvModel::refresh()
{
  beginResetModel();
  readFile();
  endResetModel();
}

void CCsvModel::readFile()
{
  m_table.clear();
  m_title.clear();
  m_columns = 0;
  QStringList list_raw;
  QStringList list;

  if (m_importCsv)
  {
      if (m_importCsv->openFile())
      {
        if (m_importCsv->getIsTitle())
            {
                m_title = m_importCsv->getTitle();
                m_importCsv->readLine();
            }
        DEBUG_WITH_LINE << m_fileName;
        while(m_importCsv->readLine())
            {
                list_raw.clear();
                list.clear();
                list_raw = m_importCsv->getBuff();
                Q_FOREACH(auto s, list_raw)
                    list.append(s.trimmed());
                m_table.append(list);
                if (m_columns < list.count())
                  m_columns = list.count();
            }
        m_importCsv->closeFile();
      }
  }
}

QStringList CCsvModel::getHeaders()
{
  return m_title;
}

int CCsvModel::getTableSize()
{
  return m_table.count();
}

QStringList CCsvModel::getColumnForImport()
{
  QStringList list;

  list.clear();
  list << tr("BRAK");
  if (m_isTitle)
     list << this->getHeaders();
  else
     {
        for(int i = 0; i < m_columns ; i++)
            list << QString("%1").arg(i+1);
     }
  return list;
}

QVariant CCsvModel::getValueForImport(int row, const QString &column)
{
    int col;

    if (m_isTitle)
       col = m_title.indexOf(column);
    else
       col = column.toInt() - 1;

    QModelIndex index = this->index(row, col);
    if (index.isValid())
        return this->data(index);
    else
        return QVariant();
}
