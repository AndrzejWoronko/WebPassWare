#ifndef LAYOUTS_H
#define LAYOUTS_H

#include "Global.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>

namespace wpw {

#ifdef Q_WS_WIN

inline constexpr int DEFAULT_LAYOUT_SPACING = 6;
inline constexpr int DEFAULT_LAYOUT_MARGIN = 11;

#else

inline constexpr int DEFAULT_LAYOUT_SPACING = 6;
inline constexpr int DEFAULT_LAYOUT_MARGIN = 9;

#endif

class CVBoxLayout : public QVBoxLayout
{
public:
    CVBoxLayout(QWidget *parent = nullptr);
    ~CVBoxLayout() { DEBUG_WITH_LINE << "~dtor "; }
};

class CHBoxLayout : public QHBoxLayout
{
public:
    CHBoxLayout(QWidget *parent = nullptr);
    ~CHBoxLayout() { DEBUG_WITH_LINE << "~dtor "; }
};

class CGridLayout : public QGridLayout
{
public:
    CGridLayout(QWidget *parent = nullptr);
    ~CGridLayout() { DEBUG_WITH_LINE << "~dtor "; }
};

class CFormLayout : public QFormLayout
{
public:
    CFormLayout(QWidget *parent = nullptr);
    ~CFormLayout() { DEBUG_WITH_LINE << "~dtor "; }
};

} // namespace wpw

#endif // LAYOUTS_H
