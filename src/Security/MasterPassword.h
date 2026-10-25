#ifndef MASTERPASSWORD_H
#define MASTERPASSWORD_H

#include <QString>

namespace wpw::MasterPassword {

// The master password opens the program. Only a salted PBKDF2-SHA256 hash of
// its UTF-8 bytes is stored:
//   pbkdf2-sha256$<iterations>$<salt, base64>$<key, base64>
// Older versions stored an unsalted MD5 hex digest; it is still accepted so
// that existing users can log in, and is replaced at the next login.

constexpr int kIterations = 100000;

QString hash(const QString &password);
bool verify(const QString &password, const QString &stored);
// True for a hash in the old MD5 form.
bool needsRehash(const QString &stored);

} // namespace wpw::MasterPassword

#endif // MASTERPASSWORD_H
