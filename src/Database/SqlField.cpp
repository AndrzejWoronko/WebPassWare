#include "SqlField.h"

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
        (getTypeName() == QString("Indicator")) || (getType() == QVariant::Double) || (getType() == QVariant::Int))
        return true;
    else
        return  false;
}

bool CSqlField::isDateField()
{
    if ((getType() == QVariant::Date) || (getType() == QVariant::DateTime))
        return true;
    else
        return  false;
}

bool CSqlField::positionLessThan(CSqlField *f1, CSqlField *f2)
{
    return f1->getPos() < f2->getPos();
}

QVariant::Type CSqlField::recognizeString2Type(const QString &type)
{
  if (type == "string")
      return QVariant::String;
  else if (type == "long")
      return QVariant::Int;
  else if (type == "int")
      return QVariant::Int;
  else if (type == "double")
      return QVariant::Double;
  else if (type == "date")
      return QVariant::Date;
  else if (type == "data")
      return QVariant::Date;
  else if (type == "ilosc")
      return QVariant::Double;
  else if (type == "wartosc")
      return QVariant::Double;
  else if (type == "datetime")
      return QVariant::DateTime;
  else if (type == "time")
      return QVariant::Time;
  else if (type == "czas")
      return QVariant::DateTime;
  else if (type == "quantity")
      return QVariant::Double;
  else if (type == "amount")
      return QVariant::Double;
  else if (type == "bool")
      return QVariant::Bool;
  else if (type == "check")
      return QVariant::Bool;
  else if (type == "Value")
      return QVariant::Double;
  else if (type == "Price")
      return QVariant::Double;
  else if (type == "Amount")
      return QVariant::Double;
  else if (type == "Indicator")
      return QVariant::Double;
  else if (type == "Percent")
      return QVariant::Double;
  else if (type == "Varchar255")
      return QVariant::String;
  else if (type == "Color")
      return QVariant::String;
  else if (type == "Timestamp")
      return QVariant::DateTime;
  else
      return QVariant::String;
}

QString CSqlField::recognizeType2String(QVariant::Type type)
{
  switch (type)
  {
  case QVariant::String:
      return QString("string");
  case QVariant::Int:
      return QString("long");
  case QVariant::Double:
      return QString("double");
  case QVariant::Date:
      return QString("data");
  case QVariant::DateTime:
      return QString("datetime");
  case QVariant::Time:
      return QString("time");
  case  QVariant::Bool:
      return QString("bool");
  default: //Domyślny typ to string
      return QString("string");
  }
  return QString();
}
