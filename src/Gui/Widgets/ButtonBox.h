#ifndef BUTTONBOX_H
#define BUTTONBOX_H

#include "Global.h"
#include <QDialogButtonBox>
#include <QPushButton>

namespace wpw {

class CButtonBox : public QDialogButtonBox
{
Q_OBJECT

    ADD_PTR_PRO(QPushButton, m_cancel_button, getCancelButton)
    ADD_PTR_PRO(QPushButton, m_save_button, getSaveButton)
public:
    CButtonBox(QWidget *parent = nullptr);
};

class CButtonBoxSave : public CButtonBox
{
Q_OBJECT

public:
    CButtonBoxSave(QWidget *parent = nullptr);
};

class CButtonBoxChoice : public CButtonBox
{
Q_OBJECT

public:
    CButtonBoxChoice(QWidget *parent = nullptr);
};

class CButtonBoxExport : public CButtonBox
{
Q_OBJECT

public:
    CButtonBoxExport(QWidget *parent = nullptr);
};

class CButtonBoxOk : public CButtonBox
{
Q_OBJECT

public:
    CButtonBoxOk(QWidget *parent = nullptr);
};

class CButtonBoxApplyRestore : public CButtonBox
{
Q_OBJECT

    ADD_PTR_PRO(QPushButton, m_restore_button, getRestoreButton)

public:
    CButtonBoxApplyRestore(QWidget *parent = nullptr);    

};

class CButtonBoxClose : public QDialogButtonBox
{

Q_OBJECT

    ADD_PTR_PRO(QPushButton, m_close_button, getCloseButton)

public:
    CButtonBoxClose(QWidget *parent = nullptr);   

};

} // namespace wpw

#endif // BUTTONBOX_H
