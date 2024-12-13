#include "Form.h"
#include "ButtonBox.h"
#include "MessageBox.h"

CForm::CForm(QWidget *parent) : QWidget(parent)
{
    m_initialized = false;
    m_dialog = 0;
    m_grid = new CGridLayout(this);
}

CForm::~CForm()
{    
    safe_delete(m_dialog)
    safe_delete(m_grid)
}

bool CForm::isDirty()
{
    for (int i = 0 ; i < m_fields.count() ; i++)
        {
            if (m_fields[i]->isDirty())
                return true;
        }
return false;
}

void CForm::init(QList<CFormAbstractField*> fields)
{
    m_fields = fields;
    m_initialized = true;   

    for (int i = 0 ; i < m_fields.count() ; i++)
    {
        CFormAbstractField *field = m_fields[i];

        if (!qobject_cast<QCheckBox*>(field->getWidget()))
            {
                QLabel *label = new QLabel(field->getLabel(), this);
                label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                if (m_grid)
                    m_grid->addWidget(label, i, 0);
            }
        if (m_grid)
            m_grid->addWidget(field->getWidget(), i, 1);
        field->getWidget()->setObjectName(field->getVariableName());
    }
}

bool CForm::exec(const QString &title)
{
    CDialog *m_dialog = new CDialog(parentWidget());

    m_dialog->setWindowTitle(title);

    if (!m_initialized)
        return false;

    CVBoxLayout lay(m_dialog);

    lay.addWidget(this);

    CButtonBox *buttonBox = new CButtonBox(m_dialog);
    lay.addWidget(buttonBox);

    connect(buttonBox, &CButtonBox::accepted, this, &CForm::validateForm);
    connect(buttonBox, &CButtonBox::rejected, this, &CForm::rejectForm);

    m_dialog->resize(400, m_dialog->height());
    bool ret = m_dialog->exec() == QDialog::Accepted;
    this->setParent(0);
    return ret;
}

void CForm::validateForm()
{
    QString error = QString("");

    if (m_validateCallback)
      error = m_validateCallback(m_fields);

    if (error.length() == 0)
        m_dialog->accept();
    else
        CMessageBox::OkDialogWarning(error, m_dialog, tr("Uzupełnij / popraw dane !!!"));
}

void CForm::rejectForm()
{
    if (isDirty())
       {
        if (CMessageBox::YesNoDialog(tr("Czy chcesz zapisać zmiany ?"), m_dialog, tr("Dokonano zmian")) == (CMessageBox::Yes))
            {
                this->validateForm();
            }
        else
            m_dialog->reject(); // zmiany dokonane, ale użytkowik chce anulować
       }
    else
        m_dialog->reject();
}

//void CForm::setValidateCallback(std::function<QString (QList<CFormAbstractField*>)> callback)
//{
//    m_validateCallback = callback;
//}

void CForm::setValidateCallback( validateFunc callback)
{
    m_validateCallback = callback;
}

void CForm::setComponents(QGridLayout *layout, QWidget *widget, const QString &labelValue, int row, int col, int dy, int dx, bool boldLabel, bool right_side)
{
    if (layout && widget)
    {
        if (!labelValue.isEmpty())
            {
               QLabel *label = new QLabel(labelValue, widget);
               if (boldLabel)
                   label->setText(QString("<b>%1</b>").arg(labelValue));
                   //label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
               label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
               //layout->addWidget(new QLabel(labelValue), row, col - 1, 1, 1);
               if (right_side)
                    layout->addWidget(label, row, col + 1, 1, 1);
               else
                    layout->addWidget(label, row, col - 1, 1, 1);
            }
        layout->addWidget(widget, row, col, dy, dx);
    }
}

void CForm::setComponents(QGridLayout *layout, QWidget *widget, QLabel *label, int row, int col, int dy, int dx)
{
    if (layout && widget && label)
    {
        layout->addWidget(widget, row, col, dy, dx);
        layout->addWidget(label, row, col - 1, 1, 1);
    }
}

void CForm::setComponents(QFormLayout *layout, QWidget *widget, const QString &labelValue, int row, bool boldLabel, bool right_side)
{
    if (layout && widget)
    {
        QLabel *label = new QLabel(labelValue, widget);
        if (boldLabel)
            label->setText(QString("<b>%1</b>").arg(labelValue));
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        if (row)
            {
              if (right_side)
                 layout->insertRow(row, widget, label);
              else
                 layout->insertRow(row, label, widget);
            }
        else
            {
                if (right_side)
                    layout->addRow(widget, label);
                else
                    layout->addRow(label, widget);
            }
    }
}

void CForm::setComponents(QGridLayout *layout, const QString &labelValue, int row, int col, bool boldLabel)
{
    if (layout && !labelValue.isEmpty())
    {
        QLabel *label = new QLabel(labelValue);
        if (boldLabel)
            label->setText(QString("<b>%1</b>").arg(labelValue));

        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        layout->addWidget(label, row, col, 1, 1);
    }
}

void CForm::setComponents(QFormLayout *layout, const QString &labelValue, int row, bool boldLabel)
{
    if (layout && !labelValue.isEmpty())
    {
        QLabel *label = new QLabel(labelValue);
        if (boldLabel)
            label->setText(QString("<b>%1</b>").arg(labelValue));

        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        if (row)
            layout->insertRow(row, label);
        else
            layout->addRow(label);
    }
}

void CForm::setComponents(QGridLayout *layout, QLabel *label, int row, int col)
{
    if (layout && label)
    {
        layout->addWidget(label, row, col, 1, 1);
    }
}
