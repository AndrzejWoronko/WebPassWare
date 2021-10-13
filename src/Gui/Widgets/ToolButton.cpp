#include "ToolButton.h"
#include "Style.h"

CToolButton::CToolButton(CButtonPrivate structure,  QWidget *parent, QSize iconSize) : QToolButton(parent)
{
    m_structure = structure;
    setOptions();
    if(!m_structure.isNullIcon())
        setIconPolicy(m_structure.getIcon(), iconSize);
}

void CToolButton::setOptions()
{
    this->setText(m_structure.getText());
    this->setStatusTip(m_structure.getTipText());
    this->setToolTip(m_structure.getTipText());
    this->setCursor(QCursor(Qt::PointingHandCursor));
    if (m_structure.getShape().width() != -1 && m_structure.getShape().height() != -1)
       {
          this->setMinimumSize(m_structure.getShape().width(),m_structure.getShape().height());
          this->setMaximumSize(m_structure.getShape().width(),m_structure.getShape().height());
       }
    //this->setFocusPolicy(Qt::NoFocus);
    if (!m_structure.getText().isEmpty())
       {
         m_buttonName = QString("TOOL_BUTTON_%1").arg(m_structure.getText().toUpper());
         m_buttonName.remove("&");
         this->setObjectName(m_buttonName);
       }
}

void CToolButton::setIconPolicy(const QString &iconName, QSize iconSize)
{
    QSize iconSizeLocal = iconSize;

    if (CStyle::iconFromStyleExist(iconName))
        this->setIcon(CStyle::iconFromStyle(iconName));
    else
        this->setIcon(QIcon::fromTheme(iconName));


    if (iconSizeLocal.isEmpty())
       {
         int size = m_structure.getShape().height() * 0.7;
         iconSizeLocal = QSize(size, size);
       }

    if(!m_structure.getText().isEmpty())
        this->setIconSize(iconSize);
    else
        this->setIconSize(QSize(BUTTON_ICON_MIN_SIZE_DX, BUTTON_ICON_MIN_SIZE_DY));
}

void CToolButton::setTopBottomLayout()
{
    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void CToolButton::setIconFromTheme(const QString &iconName)
{
    this->setIcon(QIcon::fromTheme(iconName));
}

QString CToolButton::getText(void)
{
    return m_structure.getText();
}

