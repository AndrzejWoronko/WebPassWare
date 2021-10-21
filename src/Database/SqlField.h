#ifndef CSQLFIELD_H
#define CSQLFIELD_H

#include "Global.h"
#include <QMetaType>
#include <QSqlRecord>
#include <QSqlField>

#define FIELD_NULL   0x1
#define FIELD_KEY    0x2
#define FIELD_AUTO   0x4
#define FIELD_FILTER_WHERE 0x8
#define FIELD_FILTER_HAVING 0x10
#define FIELD_SELECT_AS_QUOTE 0x20   //Dodanie cudzusłow do selecta '' np select 'adasdfasd' AS '1'

class CSqlField
{
    //Typ danych
    ADD_FIELD(QVariant::Type, m_type, getType, setType)
    ADD_FIELD(QString, m_typeName, getTypeName, setTypeName)
    //Nazwa kolumny widoku
    ADD_FIELD(QString, m_name, getName, setName)
    //Opis w hint w nagłówku
    ADD_FIELD(QString, m_description, getDescription, setDescription)
    //Pozycja w widoku
    ADD_FIELD(qint32, m_position, getPos, setPos)
    //Precyzja dla danych liczbowych
    ADD_FIELD(qint32, m_precision, getPrecision, setPrecision)
    //Długość pola
    ADD_FIELD(qint32, m_lenght, getLenght, setLenght)
    //Flagi pola
    ADD_FIELD_FLAG(qint64, m_flags, getFlags, setFlags, appendFlags, clearFlags)
    //Definicja w zapytaniu( nazwa pola lub )
    ADD_FIELD(QString, m_sqlDescription, getSqlDescription, setSqlDescription)

public:
    CSqlField();

    CSqlField(const QString &type, const QString &name, const QString &description, qint32 position = -1,
                         qint32 precision = 0, qint32 lenght =-1, qint64 flags = FIELD_FILTER_WHERE,
                         const QString &sql_description = QString());
    ~CSqlField(void);

    bool isNumericField();
    bool isDateField();
    static bool positionLessThan(CSqlField *f1, CSqlField *f2);

    static QVariant::Type recognizeString2Type(const QString &type);
    static QString recognizeType2String(QVariant::Type type);
};

//Lista z polami do wyświetlania
//            nazwa kolumny
typedef QHash<QString, CSqlField*> SqlFieldsDesc;


#endif // CSQLFIELD_H
