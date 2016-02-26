#ifndef SDEFINES_H
#define SDEFINES_H

class SLogWidget;
class SConfig;
enum class ILogLevel{ERR, WRN, INF, DBG, QT};

extern SLogWidget* SLOGGER;
extern SConfig* SCONFIG;

#define logE(ms) SLOGGER->log(ILogLevel::ERR, Q_FUNC_INFO, ms)
#define logW(ms) SLOGGER->log(ILogLevel::WRN, Q_FUNC_INFO, ms)
#define logI(ms) SLOGGER->log(ILogLevel::INF, Q_FUNC_INFO, ms)
#define logD(ms) SLOGGER->log(ILogLevel::DBG, Q_FUNC_INFO, ms)

#define cHas(k)   SCONFIG->has(k)
#define cGet(k)   SCONFIG->get(k).toString()
#define cSet(k,v) SCONFIG->set(k, s)

#define ELOG_DATE_FORMAT "HH:mm:ss" // "HH:mm:ss dd.MM.yyyy"

#include <QtCore>
#include <QtNetwork>
#include <QtWidgets>
#include <sutils.h>
#endif // SDEFINES_H
