#ifndef CSVCODEC_H
#define CSVCODEC_H

#include <QByteArray>
#include <QString>

namespace wpw {

// Converter name (QStringDecoder/QStringEncoder) for an encoding offered in
// the CSV dialogs. "DOS/OS2-852" is the dialogs' label for code page 852,
// which ICU knows as "IBM852".
QByteArray csvCodecName(const QString &name);

} // namespace wpw

#endif // CSVCODEC_H
