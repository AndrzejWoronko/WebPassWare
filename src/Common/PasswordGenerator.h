#ifndef CPASSWORDGENERATOR_H
#define CPASSWORDGENERATOR_H

#include "Global.h"

typedef QVector<QChar> PasswordGroup;

class CPasswordGenerator
{

    ADD_FIELD(quint32, m_length, getLength, setLength)

public:
    enum CharClass
    {
        LowerLetters      = 0x1,
        UpperLetters      = 0x2,
        Numbers           = 0x4,
        SpecialCharacters = 0x8
    };
    Q_DECLARE_FLAGS(CharClasses, CharClass)

    enum GeneratorFlag
    {
        ExcludeLookAlike   = 0x1,
        CharFromEveryGroup = 0x2
    };
    Q_DECLARE_FLAGS(GeneratorFlags, GeneratorFlag)

public:
    CPasswordGenerator();

    void setLength(int length);
    void setCharClasses(const CharClasses& classes);
    void setFlags(const GeneratorFlags& flags);

    bool isValid() const;

    QString generatePassword() const;

private:
    QVector<PasswordGroup> passwordGroups() const;
    quint32 numCharClasses() const;

    CharClasses m_classes;
    GeneratorFlags m_flags;
};

#endif // CPASSWORDGENERATOR_H
