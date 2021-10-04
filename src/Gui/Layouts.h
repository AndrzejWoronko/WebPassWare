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
};

class CHBoxLayout : public QHBoxLayout
{
public:
    CHBoxLayout(QWidget *parent = NULL);
};

class CGridLayout : public QGridLayout
{
public:
    CGridLayout(QWidget *parent = NULL);
};

class CFormLayout : public QFormLayout
{
public:
    CFormLayout(QWidget *parent = NULL);
};

#endif // LAYOUTS_H
