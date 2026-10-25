#include "CsvFormat.h"

namespace wpw::Csv {

bool splitRecord(const QString &text, QChar delimiter, QStringList *fields)
{
    const QChar quote(u'"');
    fields->clear();
    QString field;
    bool inQuotes = false;
    bool fieldStart = true;
    for (qsizetype i = 0; i < text.size(); ++i)
    {
        const QChar c = text.at(i);
        if (inQuotes)
        {
            if (c == quote && i + 1 < text.size() && text.at(i + 1) == quote)
            {
                field.append(quote);
                ++i;
            }
            else if (c == quote)
            {
                inQuotes = false;
            }
            else
            {
                field.append(c);
            }
            continue;
        }
        if (c == quote && fieldStart)
        {
            inQuotes = true;
            fieldStart = false;
        }
        else if (c == delimiter)
        {
            fields->append(field);
            field.clear();
            fieldStart = true;
        }
        else
        {
            field.append(c);
            fieldStart = false;
        }
    }
    // An open quoted field is kept as read so far: a file that ends inside
    // quotes still gives its text.
    fields->append(field);
    return !inQuotes;
}

QString quoteField(const QString &field, QChar delimiter, bool always)
{
    const QChar quote(u'"');
    const bool needed = field.contains(delimiter) || field.contains(quote) || field.contains(u'\n') || field.contains(u'\r');
    if (!needed && !always)
    {
        return field;
    }
    QString quoted = field;
    quoted.replace(quote, QStringLiteral("\"\""));
    return quote + quoted + quote;
}

} // namespace wpw::Csv
