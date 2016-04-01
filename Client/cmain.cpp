#include "cwgt.h"
#include <QApplication>

void messages(QtMsgType type, const QMessageLogContext& cont, const QString& msg) {
	Q_UNUSED(cont);

	switch(type){
		case QtDebugMsg:    CLOGGER->log(ILogLevel::QT, "QtD", msg); break;
		case QtWarningMsg:  CLOGGER->log(ILogLevel::QT, "QtW", msg); break;
		case QtCriticalMsg: CLOGGER->log(ILogLevel::QT, "QtC", msg); break;
		case QtFatalMsg:    CLOGGER->log(ILogLevel::QT, "QtF", msg); break;
	}
}

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	CLOGGER = new CLogWidget;
	qInstallMessageHandler(messages);

	CCONFIG = new CConfig;

	Wgt w;
	w.show();

	return a.exec();
}

