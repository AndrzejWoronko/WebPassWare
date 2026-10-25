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

} // namespace wpw
