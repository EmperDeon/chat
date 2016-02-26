#include "swgt.h"
#include <QApplication>

void messages(QtMsgType type, const QMessageLogContext& cont, const QString& msg) {
	Q_UNUSED(cont);

	switch(type){
		case QtDebugMsg:    SLOGGER->log(ILogLevel::QT, "QtD", msg); break;
		case QtWarningMsg:  SLOGGER->log(ILogLevel::QT, "QtW", msg); break;
		case QtCriticalMsg: SLOGGER->log(ILogLevel::QT, "QtC", msg); break;
		case QtFatalMsg:    SLOGGER->log(ILogLevel::QT, "QtF", msg); break;
	}
}

int main(int argc, char *argv[]){
	qInstallMessageHandler(messages);
	QApplication a(argc, argv);

	SLOGGER = new SLogWidget;
	SCONFIG = new SConfig;

	Wgt w;
	w.show();

	return a.exec();
}
