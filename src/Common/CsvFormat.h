#ifndef CSVFORMAT_H
#define CSVFORMAT_H

#include <QChar>
#include <QString>
#include <QStringList>

namespace wpw::Csv {

// RFC 4180 fields: a field in double quotes may hold the delimiter, line
// breaks and quotes written twice.

// Splits one record into \a fields. Returns false while a quoted field is
// still open at the end of \a text: the record goes on in the next line
// (\a fields then holds what was read so far).
bool splitRecord(const QString &text, QChar delimiter, QStringList *fields);

// \a field as written to a file: in quotes (with quotes doubled) when it
// holds the delimiter, a quote or a line break, or when \a always is set.
QString quoteField(const QString &field, QChar delimiter, bool always = false);

} // namespace wpw::Csv

#endif // CSVFORMAT_H
