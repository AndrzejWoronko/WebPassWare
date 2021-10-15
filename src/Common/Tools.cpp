#include "Tools.h"
#include <QMap>

#ifdef Q_OS_LINUX
#include <QFile>
#include <QRegularExpression>
#else

#ifdef Q_OS_WIN32
#include "windows.h"
#include "psapi.h"
#else

#ifdef Q_OS_MAC
#include <mach/mach.h>
#endif // Q_OS_MAC

#endif // Q_OS_WIN32
#endif // Q_OS_LINUX


Tools::Tools()
{

}

/*******************************************************************************************************************
 Poprawne zaokrąglanie poprawione
 QString::number niepoprawnie zaokrągla 5 na miejscach powyżej precyzji np 1.455 zaokrągla na 1.45 zamiast na 1.46
********************************************************************************************************************/
QString Tools::round_prec(double n, int prec)
{
  qint64 int_value = 0;
  double value = 0.00;
  qreal wsk = qPow(10, prec);

  if (wsk > 0)
    {
     int_value = qRound64( n * wsk);
     value = int_value / wsk;
    }
  else
     value = n;

  return QString::number(value, 'f', prec);
}

double Tools::round_prec_double(double n, int prec)
{
return round_prec(n, prec).toDouble();
}

QString Tools::double2NiceString(double n, int prec)
{
    QString str;
    int pos_dot = prec + 1;
    int pos_tousend = 3;

    str = QString::number(n, 'f', prec);
    str.replace(QChar('.'), QChar(',')); //zamiana . na ,
    if (qAbs(n) >= 1000)
        str.insert(str.length()-(pos_dot+pos_tousend), QChar(' '));
    if (qAbs(n) >= 1000000)
        str.insert(str.length()-(pos_dot+pos_tousend+1), QChar(' '));
    if (qAbs(n) >= 1000000000)
        str.insert(str.length()-(pos_dot+pos_tousend+2), QChar(' '));
    if (qAbs(n) >= 1000000000000)
        str.insert(str.length()-(pos_dot+pos_tousend+3), QChar(' '));
    return str;
}

QString Tools::double2NiceString2Lines(double n, int prec)
{
    QString str;

    str = double2NiceString(n, prec);
    if (qAbs(n) >= 1000)                 //Przeniesienie po przecinku do 2 linii
        str.insert(str.length()-prec, QChar('\n'));
    return str;
}

#ifdef Q_OS_LINUX
/*
VmPeak:	Peak virtual memory usage
VmSize:	Current virtual memory usage
VmLck:	Current mlocked memory
VmHWM:	Peak resident set size
VmRSS:	Resident set size
VmData:	Size of "data" segment
VmStk:	Size of stack
VmExe:	Size of "text" segment
VmLib:	Shared library usage
VmPTE:	Pagetable entries size
VmSwap:	Swap space used
*/
qint64 Tools::getMemoryUsage()
{
    //static const QRegularExpression re("VmSize\\:\\s+(\\d+)\\s+(\\w+)");
    static const QRegularExpression re("VmRSS\\:\\s+(\\d+)\\s+(\\w+)");

    QFile file("/proc/self/status");
    if (!file.open(QIODevice::ReadOnly))
        return -1;

    QString contents = file.readAll();
    QRegularExpressionMatch match = re.match(contents);
    if (!match.hasMatch())
        return -1;

    bool ok;
    qint64 result = match.captured(1).toLongLong(&ok);
    if (!ok)
        return -1;

    QString unit = match.captured(2).toLower();

    if (unit == "gb")
        return result * 1024 * 1024 * 1024;

    if (unit == "mb")
        return result * 1024 * 1024;

    if (unit == "kb")
        return result * 1024;

    return result;
}

#else
#ifdef Q_OS_WIN32

qint64 Tools::getMemoryUsage()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
    //return pmc.PrivateUsage;
}

#else
#ifdef Q_OS_MAC
/*
 struct task_basic_info_32 {
        integer_t       suspend_count;  // suspend count for task
        natural_t       virtual_size;   // virtual memory size (bytes)
        natural_t       resident_size;  // resident memory size (bytes)
        time_value_t    user_time;      // total user run time for terminated threads
        time_value_t    system_time;    // total system run time for terminated threads
        policy_t	policy;		        // default policy for new threads
        };
 */
qint64 Tools::getMemoryUsage()
{
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (KERN_SUCCESS != task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count))
        return -1;

    return t_info.resident_size;
    //return t_info.virtual_size;
}

#else
qint64 Tools::getMemoryUsage()
{
    return -1;
}

#endif // Q_OS_MAC
#endif // Q_OS_WIN32
#endif // Q_OS_LINUX

QString Tools::getMemoryUsageToString()
{
   qint64 mem_size = Tools::getMemoryUsage();
   QString mem_size_str;

   mem_size_str = QString("%1").arg(mem_size);
   if (mem_size > 2048 && mem_size < 2097152)
      {
        mem_size_str = QString("%1 kB").arg(Tools::double2NiceString(mem_size/1000.00, 2));
      }
   else if ( mem_size >= 2097152 && mem_size < 2097152000)
      {
        mem_size_str = QString("%1 MB").arg(Tools::double2NiceString(mem_size/1000000.00, 2));
      }
   else
      {
        mem_size_str = QString("%1 GB").arg(Tools::double2NiceString(mem_size/1000000000.00, 2));
      }
   return mem_size_str;
}

QString Tools::getTagValue(const QString& src, const QString& key)
{
    QString value, s = QString(src);

    value.clear();
    int start = s.indexOf(QString("<%1>").arg(key));
    int stop = s.indexOf(QString("</%1>").arg(key));
    if (start != -1 && stop != -1)
        {
            s.truncate(stop);
            value = s.mid(start + key.length() + 2);
        }
    return value;
}

QString Tools::snakeCase(const QString& name)
{
    QString newName = name;
    if (newName.indexOf(QRegExp("^[A-Z]+$")) != -1)
        return newName.toLower();

    newName = newName.replace(QRegExp("([A-Z]+)([A-Z][a-z])"),"\\1_\\2");
    newName = newName.replace(QRegExp("([a-z])([A-Z])"), "\\1_\\2");
    return newName.toLower();
}

QString Tools::tableize(const QString& name)
{
    QString table = name;
    table = snakeCase(table);
    QStringList wordList;
    QStringList tmpList = table.split(QChar('_'));
    QString last = tmpList.last();
    tmpList.removeLast();
    wordList = tmpList;
    wordList << last;
    table = wordList.join(QChar('_'));
    return table;
}
