#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QtCore>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QVector>
#include <QHash>
#include <QDateTime>
#include <QDate>

#include "Tools.h"
#include "Singleton.h"

/**
 * @brief Deletes object and sets the pointer to null.
 *
 * Deletes object under given pointer, but only if the pointer is not null.
 * Also sets the pointer to the null after deleting is done.
 */
#define safe_delete(var) \
    if (var) \
    { \
        delete var; \
        var = nullptr; \
    }

#define qobject_safe_delete(var) \
    if (var) \
    { \
        var->deleteLater(); \
        var = nullptr; \
    }

#define ADD_FIELD_QVARIANT_PROPERTY(field, getField, setField) \
              private: \
                 Q_PROPERTY(QVariant field READ getField WRITE setField) \
              public: \
                 QVariant getField(void){return field;} \
                 void setField(const QVariant &value) {field = value;} \
              private: \
                 QVariant field;

#define ADD_FIELD_TYPE_PROPERTY(type, field, getField, setField) \
              private: \
                 Q_PROPERTY(type field READ getField WRITE setField) \
              public: \
                 type getField(void){return field;} \
                 void setField(const type &value) {field = value;} \
              private: \
                 type field;

#define ADD_FIELD(type, field, getField, setField) \
              public: \
                 type getField(void){return field;} \
                 void setField(const type &value) {field = value;} \
              private: \
                 type field;

#define ADD_PTR_FIELD(type, field, getField, setField) \
              public: \
                 type* getField(void){return field;} \
                 void setField(type *value) {field = value;} \
              private: \
                 type* field = nullptr;

#define ADD_FIELD_FLAG(type, field, getField, setField, appendField, clearField) \
              public: \
                 type getField(void){return field;} \
                 void setField(const type &value) {field = value;} \
                 void appendField(const type &value) {field |= value;} \
                 void clearField(const type &value) {field &= ~value;} \
              private: \
                 type field;

#define ADD_PTR_PROPERTY(type, name, getName) \
             private: \
                  Q_PROPERTY(type* name READ getName) \
             public: \
                  type* getName(void){return name;} \
             private: \
                  type* name = nullptr;

#define ADD_PTR(type, name, getName) \
             public: \
                  type* getName(void){return name;} \
             private: \
                  type* name = nullptr;

#define ADD_PTR_PRO(type, name, getName) \
             public: \
                  type* getName(void){return name;} \
             protected: \
                  type* name = nullptr;

#define CLEAR_PTR_LIST(list) \
    Q_FOREACH(auto ptr, list) \
              safe_delete(ptr);\
              list.clear();

#define RET_OK       0
#define RET_ERROR   -1

#define DATE_FORMAT "yyyy-MM-dd"
#define MONTH_FORMAT "yyyy-MM"
#define YEAR_FORMAT "yyyy"
#define DATETIME_FORMAT "yyyy-MM-dd HH:mm:ss"
#define DATETIME_FORMAT_SHORT "yyyy-MM-dd HH:mm"
#define TIME_FORMAT "HH:mm:ss"
//Maksymalna data
#define MAX_DATE "2069-12-31"
#define MAX_DATE_TIME "2069-12-31 23:59:59"


#define QFUNC_INFO_WITH_LINE Q_FUNC_INFO << ":[" << __LINE__ << "]"
#define QFUNC_INFO_WITH_LINE_ERROR QFUNC_INFO_WITH_LINE << "ERROR:"
#define QFUNC_INFO_LINE Q_FUNC_INFO << ":[" << __LINE__ << "] : "
#define QFUNC_INFO_DATE_LINE QDateTime::currentDateTime().toString(DATETIME_FORMAT) << Q_FUNC_INFO << ":[" << __LINE__ << "] : "

//Do debugowania
#define DEBUG_WITH_LINE qDebug() << QFUNC_INFO_WITH_LINE
#define WARNING_WITH_LINE qWarning() << QFUNC_INFO_WITH_LINE
#define ERROR_WITH_LINE qCritical() << QFUNC_INFO_WITH_LINE
#define FATAL_WITH_LINE qFatal() << QFUNC_INFO_WITH_LINE

//Do logowania
#define LOG_DATE_LINE QDateTime::currentDateTime().toString(DATETIME_FORMAT) << " : "
#define LOG_DATE_LINE_QFUNC QDateTime::currentDateTime().toString(DATETIME_FORMAT) << " : " << Q_FUNC_INFO
#define LOG_DATE_LINE_QFUNC_LINE QDateTime::currentDateTime().toString(DATETIME_FORMAT) << Q_FUNC_INFO << ":[" << __LINE__ << "] : "

typedef struct
{
  qint64 value;
  qint8 decimal;
}STDecimal;

Q_DECLARE_METATYPE(STDecimal)

#define ICONS_PATH ":/icons/50px"
#define STYLE_PATH ":/style"
#define ICON(name) QString(name)
#define BUTTON_ICON_MIN_SIZE_DX 22
#define BUTTON_ICON_MIN_SIZE_DY 22


typedef double Price;
//Q_DECLARE_METATYPE(Price)
typedef double Indicator;
//Q_DECLARE_METATYPE(Indicator)
typedef double Percent;
//Q_DECLARE_METATYPE(Percent)
typedef double Amount;
//Q_DECLARE_METATYPE(Amount)
typedef double Value;
//Q_DECLARE_METATYPE(Value)
typedef QString Varchar255;
//Q_DECLARE_METATYPE(Varchar255)
typedef QDateTime Timestamp;
//Q_DECLARE_METATYPE(Timestamp)
typedef QString Color;
//Q_DECLARE_METATYPE(Color)

typedef QPair<QString, QString> PkNames;
Q_DECLARE_METATYPE(PkNames)

typedef QPair<qint64, qint16> PkValues;
Q_DECLARE_METATYPE(PkValues)

typedef QPair<QString, QVariant> SqlValue;
Q_DECLARE_METATYPE(SqlValue)

#define BZERO(a) memset(a, 0, sizeof(*a));
#define BZERO_PTR(a) memset(a, 0, sizeof(*a));
#define BZERO_REF(a) memset(&a, 0, sizeof(a));

#ifdef NULL
#undef NULL
#define NULL nullptr
#endif

#endif // GLOBAL_H

