#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QDate>
#include <QString>
#include <QtMath>
#include <QtGlobal>

#define DIGIT_ONLY_REG_EXP "^[0-9]*$"

class Tools
{

public:
    Tools();

    static QString double2NiceString(double n, int prec);
    static QString double2NiceString2Lines(double n, int prec);

    static QString round_prec(double n, int prec);
    static double round_prec_double(double n, int prec);

    static qint64 getMemoryUsage();
    static QString getMemoryUsageToString();

    static QString getTagValue(const QString& src, const QString& key);

    /**
     * Convert to snake case
     */
    static QString snakeCase(const QString& name);
    /**
     * Convert a string into a table name
     *
     * @return a version of the string useful as a database table name
     */
    static QString tableize(const QString& s);

    static void sleep(int ms);

    static void wait(int ms);
};

#endif // TOOLS_H
