#ifndef DEFINES_H
#define DEFINES_H

class CLogWidget;
class CConfig;
enum class ILogLevel{ERR, WRN, INF, DBG, QT};

extern CLogWidget* CLOGGER;
extern CConfig* CCONFIG;

#define logE(ms) CLOGGER->log(ILogLevel::ERR, Q_FUNC_INFO, ms)
#define logW(ms) CLOGGER->log(ILogLevel::WRN, Q_FUNC_INFO, ms)
#define logI(ms) CLOGGER->log(ILogLevel::INF, Q_FUNC_INFO, ms)
#define logD(ms) CLOGGER->log(ILogLevel::DBG, Q_FUNC_INFO, ms)

#define cHas(k) CCONFIG->has(k)
#define cGet(k) CCONFIG->get(k).toString()
#define cSet(k,v) CCONFIG->set(k, s)

#define ELOG_DATE_FORMAT "HH:mm:ss" // "HH:mm:ss dd.MM.yyyy"

#include <QtCore>
#include <QtNetwork>
#include <QtWidgets>
#include <cutils.h>
#endif // DEFINES_H
