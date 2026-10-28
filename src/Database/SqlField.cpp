#include "SqlField.h"

namespace wpw {

CSqlField::CSqlField()
{

}

CSqlField::CSqlField(const QString &type, const QString &name, const QString &description, qint32 position,
                                           qint32 precision, qint32 lenght, qint64 flags, const QString &sql_description)
{
    this->setTypeName(type);
    this->setType(recognizeString2Type(type));
    this->setName(name);
    this->setDescription(description);
    this->setLenght(lenght);
    this->setPos(position);
    this->setPrecision(precision);
    this->setFlags(flags);
    this->setSqlDescription(sql_description);
}

CSqlField::~CSqlField(void)
{
}

bool CSqlField::isNumericField()
{
    if ((getTypeName() == QString("Price")) || (getTypeName() == QString("Value")) || (getTypeName() == QString("Percent")) ||
        (getTypeName() == QString("Indicator")) || (getType() == QMetaType::Double) || (getType() == QMetaType::Int))
        return true;
    else
        return  false;
}

bool CSqlField::isDateField()
{
    if ((getType() == QMetaType::QDate) || (getType() == QMetaType::QDateTime))
        return true;
    else
        return  false;
}

bool CSqlField::positionLessThan(CSqlField *f1, CSqlField *f2)
{
    return f1->getPos() < f2->getPos();
}

QMetaType::Type CSqlField::recognizeString2Type(const QString &type)
{
  if (type == "string")
      return QMetaType::QString;
  else if (type == "long")
      return QMetaType::Int;
  else if (type == "int")
      return QMetaType::Int;
  else if (type == "double")
      return QMetaType::Double;
  else if (type == "date")
      return QMetaType::QDate;
  else if (type == "data")
      return QMetaType::QDate;
  else if (type == "ilosc")
      return QMetaType::Double;
  else if (type == "wartosc")
      return QMetaType::Double;
  else if (type == "datetime")
      return QMetaType::QDateTime;
  else if (type == "time")
      return QMetaType::QTime;
  else if (type == "czas")
      return QMetaType::QDateTime;
  else if (type == "quantity")
      return QMetaType::Double;
  else if (type == "amount")
      return QMetaType::Double;
  else if (type == "bool")
      return QMetaType::Bool;
  else if (type == "check")
      return QMetaType::Bool;
  else if (type == "Value")
      return QMetaType::Double;
  else if (type == "Price")
      return QMetaType::Double;
  else if (type == "Amount")
      return QMetaType::Double;
  else if (type == "Indicator")
      return QMetaType::Double;
  else if (type == "Percent")
      return QMetaType::Double;
  else if (type == "Varchar255")
      return QMetaType::QString;
  else if (type == "Color")
      return QMetaType::QString;
  else if (type == "Timestamp")
      return QMetaType::QDateTime;
  else
      return QMetaType::QString;
}

QString CSqlField::recognizeType2String(QMetaType::Type type)
{
  switch (type)
  {
  case QMetaType::QString:
      return QString("string");
  case QMetaType::Int:
      return QString("long");
  case QMetaType::Double:
      return QString("double");
  case QMetaType::QDate:
      return QString("data");
  case QMetaType::QDateTime:
      return QString("datetime");
  case QMetaType::QTime:
      return QString("time");
  case  QMetaType::Bool:
      return QString("bool");
  default: //Domyślny typ to string
      return QString("string");
  }
  return QString();
}

} // namespace wpw
