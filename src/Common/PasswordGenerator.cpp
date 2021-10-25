#include "PasswordGenerator.h"
#include <QRandomGenerator>

CPasswordGenerator::CPasswordGenerator() : m_length(0), m_classes(QFlag(0)), m_flags(QFlag(0))
{
}

void CPasswordGenerator::setCharClasses(const CharClasses& classes)
{
    m_classes = classes;
}

void CPasswordGenerator::setFlags(const GeneratorFlags& flags)
{
    m_flags = flags;
}

QString CPasswordGenerator::generatePassword() const
{
    Q_ASSERT(isValid());

    const QVector<PasswordGroup> groups = passwordGroups();

    QVector<QChar> passwordChars;
    Q_FOREACH(auto group, groups)
    {
        Q_FOREACH(auto ch, group)
        {
            passwordChars.append(ch);
        }
    }

    QString password;

    if (m_flags & CharFromEveryGroup)
    {
        for (int i = 0; i < groups.size(); i++)
        {
            //int pos = randomGen()->randomUInt(groups[i].size());
            int pos = QRandomGenerator::global()->bounded(groups[i].size());
            password.append(groups[i][pos]);
        }

        for (quint32 i = groups.size(); i < m_length; i++)
        {
            //int pos = randomGen()->randomUInt(passwordChars.size());
            int pos = QRandomGenerator::global()->bounded(passwordChars.size());
            password.append(passwordChars[pos]);
        }

        // shuffle chars
        for (int i = (password.size() - 1); i >= 1; i--)
        {
            //int j = randomGen()->randomUInt(i + 1);
            int j = QRandomGenerator::global()->bounded(i + 1);

            QChar tmp = password[i];
            password[i] = password[j];
            password[j] = tmp;
        }
    }
    else
    {
        for (quint32 i = 0; i < m_length; i++)
        {
            //int pos = randomGen()->randomUInt(passwordChars.size());
            int pos = QRandomGenerator::global()->bounded(passwordChars.size());

            password.append(passwordChars[pos]);
        }
    }

    return password;
}

bool CPasswordGenerator::isValid() const
{
    if (m_classes == 0)
    {
        return false;
    }
    else if (m_length == 0)
    {
        return false;
    }

    if ((m_flags & CharFromEveryGroup) && (m_length < numCharClasses()))
    {
        return false;
    }

    return true;
}

QVector<PasswordGroup> CPasswordGenerator::passwordGroups() const
{
    QVector<PasswordGroup> passwordGroups;

    if (m_classes & LowerLetters)
    {
        PasswordGroup group;

        for (int i = 97; i < (97 + 26); i++)
        {
            if ((m_flags & ExcludeLookAlike) && (i == 108))
            { // "l"
                continue;
            }
            group.append(i);
        }

        passwordGroups.append(group);
    }
    if (m_classes & UpperLetters)
    {
        PasswordGroup group;

        for (int i = 65; i < (65 + 26); i++)
        {
            if ((m_flags & ExcludeLookAlike) && (i == 73 || i == 79))
            { // "I" and "O"
                continue;
            }
            group.append(i);
        }
        passwordGroups.append(group);
    }
    if (m_classes & Numbers)
    {
        PasswordGroup group;

        for (int i = 48; i < (48 + 10); i++)
        {
            if ((m_flags & ExcludeLookAlike) && (i == 48 || i == 49))
            { // "0" and "1"
                continue;
            }
            group.append(i);
        }

        passwordGroups.append(group);
    }
    if (m_classes & SpecialCharacters)
    {
        PasswordGroup group;

        for (int i = 33; i <= 47; i++)
        {
            group.append(i);
        }

        for (int i = 58; i <= 64; i++)
        {
            group.append(i);
        }

        for (int i = 91; i <= 96; i++)
        {
            group.append(i);
        }

        for (int i = 123; i <= 126; i++)
        {
            if ((m_flags & ExcludeLookAlike) && (i == 124))
            { // "|"
                continue;
            }
            group.append(i);
        }

        passwordGroups.append(group);
    }

    return passwordGroups;
}

quint32 CPasswordGenerator::numCharClasses() const
{
    int numClasses = 0;

    if (m_classes & LowerLetters)
    {
        numClasses++;
    }
    if (m_classes & UpperLetters)
    {
        numClasses++;
    }
    if (m_classes & Numbers)
    {
        numClasses++;
    }
    if (m_classes & SpecialCharacters)
    {
        numClasses++;
    }
    return numClasses;
}
