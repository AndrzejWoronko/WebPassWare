#ifndef FIXEDLINEEDIT_H
#define FIXEDLINEEDIT_H

#include "Global.h"
#include <QLineEdit>
#include <QStyle>

class CFixedLineEdit : public QLineEdit
{
public:

    Q_OBJECT

    Q_PROPERTY(int m_minVisibleChars READ getMinVisibleChars WRITE setMinVisibleChars)
    Q_PROPERTY(int m_maxVisibleChars READ getMaxVisibleChars WRITE setMaxVisibleChars)

   public:
       CFixedLineEdit(const QString &text, QWidget *parent = NULL);
       CFixedLineEdit(const QString &text, int max, int min = 1, QWidget *parent = NULL);

       int  getMinVisibleChars() const;
       int  getMaxVisibleChars() const;
       void setMinVisibleChars(int count);
       void setMaxVisibleChars(int count);

   private:
       int m_minVisible;
       int m_maxVisible;
};

#endif // FIXEDLINEEDIT_H
