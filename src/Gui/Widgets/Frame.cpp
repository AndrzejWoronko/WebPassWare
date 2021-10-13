#include "Frame.h"

CFrame::CFrame(QWidget *parent, Qt::WindowFlags f) : QFrame(parent, f)
{
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Plain);
}

CHLine::CHLine(QWidget *parent, Qt::WindowFlags f) : QFrame(parent, f)
{
    setFrameShape(QFrame::HLine);
    setFrameShadow(QFrame::Sunken);
}

CVLine::CVLine(QWidget *parent, Qt::WindowFlags f) : QFrame(parent, f)
{
    setFrameShape(QFrame::VLine);
    setFrameShadow(QFrame::Sunken);
}
