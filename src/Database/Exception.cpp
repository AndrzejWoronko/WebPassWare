#include "Exception.h"

namespace wpw {

CException::CException()
{

}

CException::CException(const QString &metoda, const QString &komunikat)
{
    m_metoda = metoda;
    m_komunikat = komunikat;
}

} // namespace wpw
