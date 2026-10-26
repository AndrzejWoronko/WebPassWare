#include "CsvCodec.h"

namespace wpw {

QByteArray csvCodecName(const QString &name)
{
    if (name == QLatin1String("DOS/OS2-852"))
    {
        return QByteArrayLiteral("IBM852");
    }
    return name.toLatin1();
}

QStringList csvCodecNames()
{
    return QStringList() << QString("ISO-8859-2") << QString("Windows-1250") << QString("UTF-8") << QString("DOS/OS2-852");
}

QStringList csvSeparatorNames()
{
    return QStringList() << QString("TAB") << QString("|") << QString(";") << QString(",");
}

QStringList csvDigitSigns()
{
    return QStringList() << QString(",") << QString(".");
}

QChar csvSeparator(const QString &name)
{
    if (name == QLatin1String("TAB"))
    {
        return QChar(u'\t');
    }
    return name.isEmpty() ? QChar() : name.at(0);
}

} // namespace wpw
