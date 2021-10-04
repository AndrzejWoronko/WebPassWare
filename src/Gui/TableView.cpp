#include "TableView.h"
#include "ApplicationSettings.h"

CTableView::CTableView(QWidget * parent) : QTableView(parent)
{
    m_autoResizeColumn = -1;
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
//set row color
    //this->setAlternatingRowColors(true);
    this->setAlternatingRowColors(SETT.getValue(SETTINGS_GUI_SHOW_GRID_ROW_COLORS, QVariant(true)).toBool());
//Set show grid
    //this->setShowGrid(true);
    this->setShowGrid(SETT.getValue(SETTINGS_GUI_SHOW_GRID, QVariant(true)).toBool());
    //this->setMenu();
    this->setFocusPolicy(Qt::StrongFocus);
    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);
    this->horizontalHeader()->setSortIndicatorShown(true);
    this->horizontalHeader()->setSectionsClickable(true);
    //set header
    this->setupVerticalHeader();
    this->installEventFilter(this);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //this->setContextMenuPolicy(Qt::CustomContextMenu);
#if QT_VERSION <= 0x050000
    this->horizontalHeader()->setMovable(true);
#else
    this->horizontalHeader()->setSectionsMovable(true);
#endif
    //this->setCornerButtonEnabled(true);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    //this->setStyleSheet("QTableWidget::item{ selection-background-color: red}");

    QString style = QString("QTableWidget {selection-background-color: #314159;};");
    style.append(QString("item{ background-color: blue;}; item:selected{ background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #eaeaea, stop: 1 #b2b2b2); border: 2px solid darkgrey; color:black;};"));

    this->setStyleSheet(style);
    //this->setStyleSheet(styleSheet() + QString(" item:hover { background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffa02f, stop: 1 #ca0619); color: #000000;}") );
    //this->setStyleSheet(styleSheet() + QString(" item:selected { background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffa02f, stop: 1 #d7801a);}") );
    //this->setStyleSheet(styleSheet() + QString(" focus { border: 2px solid QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffa02f, stop: 1 #d7801a);}") );
/*
  this->setStyleSheet("* {selection-background-color: #314159; gridline-color: transparent;border-top-left-radius: 10px; border-bottom-right-radius: 10px};" \
                        " item { background-color: blue }; " \
                        " item:selected { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #eaeaea, stop: 1 #b2b2b2); " \
                        " border: 10px solid darkgrey; color:black; background-color: red }; " \
                        " QTableView > QTableCornerButton::section { alternate-background-color: rgb(70, 70, 70); background-color:  rgb(39, 39, 39); " \
                        " selection-background-color: red; selection-color: red;  border-top-left-radius: 10px;} " \
                        );
*/
}

CTableView::~CTableView()
{
}

void CTableView::setupVerticalHeader(double rowHeight)
{
  if (rowHeight == -1.0)
    {
      //rowHeight = 2;
      rowHeight = SETT.getValue(SETTINGS_GUI_ROW_HEIGHT, QVariant(2)).toDouble();
    }
  rowHeight *= this->verticalHeader()->fontMetrics().height();
  this->verticalHeader()->setDefaultSectionSize(rowHeight);
  //Header size
  //Z settings
  //this->verticalHeader()->setDefaultSectionSize(20);
}

void CTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
}

//void CTableView::contextMenuEvent(QContextMenuEvent *event)
//{
//    //tableViewExtendedMenu->exec(event);
//}

void CTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
  emit currentIndexChanged(current, previous);
  QTableView::currentChanged(current, previous);
}

/*
bool CTableView::eventFilter(QObject *obj, QEvent *e)
{
  if (obj == this)
  {
    if (e->type() == e->Resize && m_autoResizeColumn != -1 && model())
       {
         resizeColumnsToContents();
       }

    if(e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease)
       {
           QKeyEvent *key = static_cast<QKeyEvent *>(e);
           if(( e->type() == QEvent::KeyRelease &&  key->key() == Qt::Key_Space))
            {
                DEBUG_WITH_LINE << "Naciśnięto spację";
                emit this->selectionModel()->selectionChanged(this->selectionModel()->selection(),this->selectionModel()->selection());
                this->goNext();
                return QObject::eventFilter(obj, e);
            }
            else if (( e->type() == QEvent::KeyPress &&  key->key() == Qt::Key_Space))
                {
                QModelIndex current = this->currentIndex();
                if (current.isValid() && current.column())
                    {
                        current = this->model()->index(current.row(), 0);
                        this->setCurrentIndex(current);
                    }
                return QObject::eventFilter(obj, e);
                }
           else if (( e->type() == QEvent::KeyPress &&  key->key() == Qt::Key_Home))
                {
                QModelIndex current = this->currentIndex();
                if (current.isValid())
                   {
                    DEBUG_WITH_LINE << "Wiersz: "<< current.row() << "Kolumna: " <<  current.column();
                   }
                this->goFirst();
                }
           else if (( e->type() == QEvent::KeyPress &&  key->key() == Qt::Key_End))
                {
                QModelIndex current = this->currentIndex();
                if (current.isValid())
                   {
                    DEBUG_WITH_LINE << "Wiersz: "<< current.row() << "Kolumna: " <<  current.column();
                   }
                this->goLast();
                }
            else if(e->type() == QEvent::KeyRelease && ((key->modifiers() & Qt::ControlModifier) && key->key() == Qt::Key_A))
                {
                    DEBUG_WITH_LINE << "Naciśnięto CTRL+A";
                    emit selectedAll();
                    emit this->selectionModel()->selectionChanged(this->selectionModel()->selection(),this->selectionModel()->selection());
                    return QObject::eventFilter(obj, e);
                }
           else if (( e->type() == QEvent::KeyPress &&  key->key() == Qt::Key_Insert))
                {
                    DEBUG_WITH_LINE << "Naciśnięto Insert";
                    emit keyInsert();
                    return QObject::eventFilter(obj, e);
                }
           else if (( e->type() == QEvent::KeyPress &&  key->key() == Qt::Key_Delete))
                {
                    DEBUG_WITH_LINE << "Naciśnięto Delete";
                    emit keyDelete();
                    return QObject::eventFilter(obj, e);
                }
           else if (( e->type() == QEvent::KeyPress &&  (key->key() == Qt::Key_Enter || key->key() == Qt::Key_Return)))
                {
                    DEBUG_WITH_LINE << "Naciśnięto Enter";
                    emit keyEnter();
                    return QObject::eventFilter(obj, e);
                }
           else
                {
                    //DEBUG_WITH_LINE << "Naciśnięto klawisz" << key->text();
                    return QObject::eventFilter(obj, e);
                }
       }
        else
       {
            return QObject::eventFilter(obj, e);
       }
  }
  return QObject::eventFilter(obj, e);
}

void CTableView::keyPressEvent(QKeyEvent *event)
{
    QTableView::keyPressEvent(event);
}

void CTableView::keyReleaseEvent(QKeyEvent *event)
{
    QTableView::keyReleaseEvent(event);
}
*/

bool CTableView::eventFilter(QObject *obj, QEvent *e)
{
  if (obj == this)
  {
    if (e->type() == e->Resize && m_autoResizeColumn != -1 && model())
       {
         resizeColumnsToContents();
       }

    if(e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease)
       {
           QKeyEvent *key = static_cast<QKeyEvent *>(e);
           if(( e->type() == QEvent::KeyRelease &&  key->key() == Qt::Key_Space))
            {
                DEBUG_WITH_LINE << "Puszczono spację";
                emit this->selectionModel()->selectionChanged(this->selectionModel()->selection(),this->selectionModel()->selection());
                this->goNext();
                return QObject::eventFilter(obj, e);
            }
            else if (( e->type() == QEvent::KeyPress &&   key->key() == Qt::Key_Space))
                {
                DEBUG_WITH_LINE << "Naciśnięto spację";
                QModelIndex current = this->currentIndex();
                if (current.isValid() && current.column())
                    {
                        current = this->model()->index(current.row(), 0);
                        this->setCurrentIndex(current);
                        this->selectRow(current.row());
                    }
                return QObject::eventFilter(obj, e);
                }
            else if((e->type() == QEvent::KeyRelease) && ((key->modifiers() & Qt::ControlModifier) && key->key() == Qt::Key_A))
               {
                   QModelIndex current = this->currentIndex();
                   DEBUG_WITH_LINE << "Puszczono CTRL+A";
                   emit selectedAll();
                   emit this->selectionModel()->selectionChanged(this->selectionModel()->selection(),this->selectionModel()->selection());
                   if (current.isValid() && current.column())
                      {
                         current = this->model()->index(current.row(), 0);
                         this->setCurrentIndex(current);
                      }
                   this->selectRow(current.row());
                   return QObject::eventFilter(obj, e);
               }
           else if((e->type() == QEvent::KeyRelease) && ((key->modifiers() & Qt::ControlModifier) && key->key() == Qt::Key_F))
              {
                  DEBUG_WITH_LINE << "Puszczono CTRL+F";
                  emit keyFind();
                  return QObject::eventFilter(obj, e);
              }
        }
    else
        {
            return QObject::eventFilter(obj, e);
        }
   }
  return QObject::eventFilter(obj, e);
}

void CTableView::keyPressEvent(QKeyEvent *event)
{
  if (event)
  {
      switch(event->key())
      {
        case Qt::Key_Home:
        {
          QModelIndex current = this->currentIndex();
          if (current.isValid())
             {
               DEBUG_WITH_LINE << "Wiersz: "<< current.row() << "Kolumna: " <<  current.column();
             }
          this->goFirst();
        }
        break;
        case Qt::Key_End:
        {
            QModelIndex current = this->currentIndex();
            if (current.isValid())
               {
                 DEBUG_WITH_LINE << "Wiersz: "<< current.row() << "Kolumna: " <<  current.column();
               }
            this->goLast();
        }
        break;
        case Qt::Key_Insert:
        {
          emit keyInsert();
        }
        break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
        {
          emit keyEnter();
        }
        break;
        case Qt::Key_Delete:
        {
          emit keyDelete();
        }
        break;
        case Qt::Key_F1:
        {
          emit keyF1();
        }
        break;
        case Qt::Key_F2:
        {
          emit keyF2();
        }
        break;
        case Qt::Key_F3:
        {
          emit keyF3();
        }
        break;
        case Qt::Key_F4:
        {
          emit keyF4();
        }
        break;
        case Qt::Key_F5:
        {
          emit keyF5();
        }
        break;
        case Qt::Key_F6:
        {
          emit keyF6();
        }
        break;
        case Qt::Key_F7:
        {
          emit keyF7();
        }
        break;
        case Qt::Key_F8:
        {
          emit keyF8();
        }
        break;
        case Qt::Key_F9:
        {
          emit keyF9();
        }
        break;
        case Qt::Key_F10:
        {
          emit keyF10();
        }
        break;
        case Qt::Key_F11:
        {
          emit keyF11();
        }
        break;
        case Qt::Key_F12:
        {
          emit keyF12();
        }
        break;
        default:
          //event->ignore();
          QTableView::keyPressEvent(event);
          break;
        }
      DEBUG_WITH_LINE << "Naciśnięto klawisz: " << event->text() << " : " << event->key();
  }
}

void CTableView::keyReleaseEvent(QKeyEvent *event)
{
  QTableView::keyReleaseEvent(event);
  DEBUG_WITH_LINE << "Puszczono klawisz: " << event->text() << " : " << event->key();
  return;
}

void CTableView::mousePressEvent(QMouseEvent *event)
{
    if (event && event->button() == Qt::LeftButton)
    {
        QModelIndex index = this->indexAt(event->pos());
        if (index.isValid() && index.column() == 0)
           {
                //DEBUG_WITH_LINE << "Naciśnięto lewy przycisk myszy idx: " << index;
                this->setFocus();
                this->setCurrentIndex(index);
                this->selectRow(index.row());
                emit selectedOne(index);
                emit this->selectionModel()->selectionChanged(this->selectionModel()->selection(),this->selectionModel()->selection());
                return;
           }
    }
    QTableView::mousePressEvent(event);
    return;
}

void CTableView::resizeColumnsToContents()
{
  QTableView::resizeColumnsToContents();

  if (m_autoResizeColumn != -1)
  {
    int width = 0;
    for (int i = 0 ; i < model()->columnCount(); i++)
    {
      if (i != m_autoResizeColumn)
        width += columnWidth(i);
    }
    if (width + this->columnWidth(m_autoResizeColumn) < this->viewport()->width())
      this->horizontalHeader()->resizeSection(m_autoResizeColumn,this->viewport()->width() - width);
  }
}

void CTableView::setAutoResizeColumn(int column)
{
    this->m_autoResizeColumn = column;
}

void CTableView::goFirst()
{
    QModelIndex current = this->currentIndex();
    this->setFocus();
    if (current.isValid())
        {
            current = this->model()->index(0, current.column());
            this->setCurrentIndex(current);
        }
}

void CTableView::goLast()
{
    QModelIndex current = this->currentIndex();
    this->setFocus();
    if (current.isValid())
        {
            current = this->model()->index(this->model()->rowCount()- 1, current.column());
            this->setCurrentIndex(current);
        }
}

void CTableView::goNext()
{
    QModelIndex current = this->currentIndex();
    this->setFocus();
    if (current.isValid() && (current.row() < this->model()->rowCount()- 1))
        {
            current = this->model()->index(current.row() + 1, current.column());
            this->setCurrentIndex(current);
        }
}

void CTableView::goPrev()
{
    QModelIndex current = this->currentIndex();
    this->setFocus();
    if (current.isValid() && current.row() > 0)
        {
            current = this->model()->index(current.row() - 1, current.column());
            this->setCurrentIndex(current);
        }
}
