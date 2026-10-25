#ifndef SCHEMA_H
#define SCHEMA_H

#include <QString>

class CDatabase;

namespace wpw::Schema {

// Version of the database file, kept in PRAGMA user_version. Files of the
// Qt 5 versions have 0. Bump it and append a step to the migrations in
// Schema.cpp for every change of the stored data.
constexpr int kVersion = 1;

// Brings the database to kVersion: creates missing tables, columns and
// indexes, then runs the migrations, each in its own transaction. Run at
// every start. Fails (and changes nothing) for a file of a newer version.
bool upgrade(CDatabase &db, QString *error);

int version(CDatabase &db);

} // namespace wpw::Schema

#endif // SCHEMA_H
