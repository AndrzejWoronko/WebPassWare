#ifndef BUTTONBOX_H
#define BUTTONBOX_H

#include "Global.h"
#include <QDialogButtonBox>
#include <QPushButton>

class CButtonBox : public QDialogButtonBox
{
Q_OBJECT

    ADD_PTR_PRO(QPushButton, m_cancel_button, getCancelButton)
    ADD_PTR_PRO(QPushButton, m_save_button, getSaveButton)
public:
    CButtonBox(QWidget *parent = NULL);
};

class CButtonBoxSave : public CButtonBox
{
Q_OBJECT

public:
    CButtonBoxSave(QWidget *parent = NULL);
};

class CButtonBoxChoice : public CButtonBox
{
Q_OBJECT

public:
    CButtonBoxChoice(QWidget *parent = NULL);
};

class CButtonBoxExport : public CButtonBox
{
Q_OBJECT

public:
    CButtonBoxExport(QWidget *parent = NULL);
};

class CButtonBoxOk : public CButtonBox
{
Q_OBJECT

public:
    CButtonBoxOk(QWidget *parent = NULL);
};

class CButtonBoxApplyRestore : public CButtonBox
{
Q_OBJECT

    ADD_PTR_PRO(QPushButton, m_restore_button, getRestoreButton)

public:
    CButtonBoxApplyRestore(QWidget *parent = NULL);    

};

class CButtonBoxClose : public QDialogButtonBox
{

Q_OBJECT

    ADD_PTR_PRO(QPushButton, m_close_button, getCloseButton)

public:
    CButtonBoxClose(QWidget *parent = NULL);   

};

#endif // BUTTONBOX_H
