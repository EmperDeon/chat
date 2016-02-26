#include "cwgt.h"
#include <QApplication>

void messages(QtMsgType type, const QMessageLogContext& cont, const QString& msg) {
	Q_UNUSED(cont);

	switch(type){
		case QtDebugMsg:    CLOGGER->log(ILogLevel::QT, "E-QtD", msg); break;
		case QtWarningMsg:  CLOGGER->log(ILogLevel::QT, "E-QtW", msg); break;
		case QtCriticalMsg: CLOGGER->log(ILogLevel::QT, "E-QtC", msg); break;
		case QtFatalMsg:    CLOGGER->log(ILogLevel::QT, "E-QtF", msg); break;
	}
}

int main(int argc, char *argv[]){
	qInstallMessageHandler(messages);
	QApplication a(argc, argv);

	CLOGGER = new CLogWidget;
	CCONFIG = new CConfig;
	CLOGGER->showMaximized();
	Wgt w;
	w.show();

	return a.exec();
}

