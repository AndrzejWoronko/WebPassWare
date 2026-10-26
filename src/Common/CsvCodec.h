#ifndef CSVCODEC_H
#define CSVCODEC_H

#include <QByteArray>
#include <QChar>
#include <QString>
#include <QStringList>

namespace wpw {

// Converter name (QStringDecoder/QStringEncoder) for an encoding offered in
// the CSV dialogs. "DOS/OS2-852" is the dialogs' label for code page 852,
// which ICU knows as "IBM852".
QByteArray csvCodecName(const QString &name);

// Choices of the CSV import and export dialogs.
QStringList csvCodecNames();
// "TAB" stands for the tab character.
QStringList csvSeparatorNames();
QStringList csvDigitSigns();
// The separator character for a choice of csvSeparatorNames(); null for "".
QChar csvSeparator(const QString &name);

} // namespace wpw

#endif // CSVCODEC_H
