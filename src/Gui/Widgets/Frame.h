#ifndef FRAME_H
#define FRAME_H

#include "Global.h"
#include <QFrame>

class CFrame : public QFrame
{
public:
    CFrame(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

};


class CHLine : public QFrame
{
public:
    CHLine(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CHLine() { DEBUG_WITH_LINE << "~dtor "; }
};

class CVLine : public QFrame
{
public:
    CVLine(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CVLine() { DEBUG_WITH_LINE << "~dtor "; }
};


#endif // FRAME_H
