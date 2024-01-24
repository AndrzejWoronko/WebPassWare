#include "Splitter.h"


CSplitter::CSplitter(const QString &splitterName,  Qt::Orientation orientation, QWidget *parent) : QSplitter(orientation, parent)
{
    setSplitterName(splitterName);
    m_splitter_interf.reset(new CSplitterState());

    //restoreSplitterState(); //Nie ma sensu restore w konstruktorze działa dopiero po dodaniu widgetów
    //setStyleSheet(QString("handle:horizontal { width: 10px; }; handle:vertical { height: 10px; };"));
    //this->setFrameShadow(QFrame::Sunken);
//    if (orientation == Qt::Horizontal)
//        this->setFrameShape(QFrame::HLine);
//    else
//        this->setFrameShape(QFrame::VLine);
//  this->setHandleWidth(10);
}

CSplitter::CSplitter(Qt::Orientation orientation, QWidget *parent) : QSplitter(orientation, parent)
{    
    m_splitter_interf.reset(new CSplitterState());

    //if (parent)
    //    setSplitterName(QString("%1_Splitter").arg(parent->objectName()));
    //else
    //    setSplitterName(QString("%1_Splitter").arg(this->objectName()));
    //restoreSplitterState(); //Nie ma sensu restore w konstruktorze działa dopiero po dodaniu widgetów
//    this->setFrameShadow(QFrame::Sunken);
//    if (orientation == Qt::Horizontal)
//       this->setFrameShape(QFrame::HLine);
//    else
//       this->setFrameShape(QFrame::VLine);
//    this->setHandleWidth(10);
}

CSplitter::~CSplitter()
{
    DEBUG_WITH_LINE << "Splitter destructor " << getSplitterName();
    saveSplitterState();
}

void CSplitter::restoreSplitterState()
{
    if (getSplitterName().isEmpty())
        return;

    if (m_splitter_interf)
        {      
        QByteArray state = m_splitter_interf->getState(getSplitterName());

        this->restoreState(state);
        DEBUG_WITH_LINE << "restore Splitter " << getSplitterName();
        }
}

void CSplitter::saveSplitterState()
{
    if (getSplitterName().isEmpty())
        return;

    if (m_splitter_interf)
    {
        QByteArray state = this->saveState();
        QByteArray oldState = m_splitter_interf->getState(getSplitterName());

        if(state != oldState)
          {
            m_splitter_interf->saveState(getSplitterName(), state);
            DEBUG_WITH_LINE << "save Splitter " << getSplitterName();
          }
    }
}
