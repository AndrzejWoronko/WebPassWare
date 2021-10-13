#ifndef FRAME_H
#define FRAME_H

#include "Global.h"
#include <QFrame>

class CFrame : public QFrame
{
public:
    CFrame(QWidget *parent = NULL, Qt::WindowFlags f = Qt::WindowFlags());

};


class CHLine : public QFrame
{
public:
    CHLine(QWidget *parent = NULL, Qt::WindowFlags f = Qt::WindowFlags());

};

class CVLine : public QFrame
{
public:
    CVLine(QWidget *parent = NULL, Qt::WindowFlags f = Qt::WindowFlags());

};


#endif // FRAME_H
