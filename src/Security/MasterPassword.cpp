#include "MasterPassword.h"

#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QStringList>

namespace wpw::MasterPassword {

namespace {

constexpr int kSaltBytes = 16;
constexpr int kKeyBytes = 32;
const QString kScheme = QStringLiteral("pbkdf2-sha256");

QByteArray derive(const QString &password, const QByteArray &salt, int iterations)
{
    return QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha256, password.toUtf8(), salt, iterations, kKeyBytes);
}

// Time independent of where the first difference is.
bool sameBytes(const QByteArray &a, const QByteArray &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    unsigned char difference = 0;
    for (qsizetype i = 0; i < a.size(); ++i)
    {
        difference |= static_cast<unsigned char>(a.at(i) ^ b.at(i));
    }
    return difference == 0;
}

bool isLegacyMd5(const QString &stored)
{
    static const QRegularExpression md5Hex(QStringLiteral("^[0-9a-f]{32}$"));
    return md5Hex.match(stored).hasMatch();
}

} // namespace

QString hash(const QString &password)
{
    QByteArray salt(kSaltBytes, '\0');
    QRandomGenerator::system()->fillRange(reinterpret_cast<quint32 *>(salt.data()), kSaltBytes / 4);
    return QStringList{kScheme, QString::number(kIterations), QString::fromLatin1(salt.toBase64()),
                       QString::fromLatin1(derive(password, salt, kIterations).toBase64())}
        .join(u'$');
}

bool verify(const QString &password, const QString &stored)
{
    if (isLegacyMd5(stored))
    {
        // The old form hashed the Latin-1 bytes; kept only to read existing settings.
        const QByteArray digest = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5).toHex();
        return sameBytes(digest, stored.toLatin1());
    }
    const QStringList parts = stored.split(u'$');
    if (parts.size() != 4 || parts.at(0) != kScheme || password.isEmpty())
    {
        return false;
    }
    bool ok = false;
    const int iterations = parts.at(1).toInt(&ok);
    if (!ok || iterations < 1)
    {
        return false;
    }
    const QByteArray salt = QByteArray::fromBase64(parts.at(2).toLatin1());
    const QByteArray key = QByteArray::fromBase64(parts.at(3).toLatin1());
    if (salt.isEmpty() || key.size() != kKeyBytes)
    {
        return false;
    }
    return sameBytes(derive(password, salt, iterations), key);
}

bool needsRehash(const QString &stored)
{
    return isLegacyMd5(stored);
}

} // namespace wpw::MasterPassword
