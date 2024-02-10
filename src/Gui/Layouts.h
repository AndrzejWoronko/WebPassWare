#ifndef LAYOUTS_H
#define LAYOUTS_H

#include "Global.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>

#ifdef Q_WS_WIN

#define DEFAULT_LAYOUT_SPACING  6
#define DEFAULT_LAYOUT_MARGIN   11

#else

#define DEFAULT_LAYOUT_SPACING  6
#define DEFAULT_LAYOUT_MARGIN   9

#endif

class CVBoxLayout : public QVBoxLayout
{
public:
    CVBoxLayout(QWidget *parent = NULL);
    ~CVBoxLayout() { DEBUG_WITH_LINE << "~dtor "; }
};

class CHBoxLayout : public QHBoxLayout
{
public:
    CHBoxLayout(QWidget *parent = NULL);
    ~CHBoxLayout() { DEBUG_WITH_LINE << "~dtor "; }
};

class CGridLayout : public QGridLayout
{
public:
    CGridLayout(QWidget *parent = NULL);
    ~CGridLayout() { DEBUG_WITH_LINE << "~dtor "; }
};

class CFormLayout : public QFormLayout
{
public:
    CFormLayout(QWidget *parent = NULL);
    ~CFormLayout() { DEBUG_WITH_LINE << "~dtor "; }
};

#endif // LAYOUTS_H
