#ifndef SUTILS_H
#define SUTILS_H
#include <sdefines.h>

struct CLogE {
	ILogLevel lv;
	QDateTime d;
	QString t, cl, ms;

	CLogE(ILogLevel lvl, QDateTime dt, QString cls, QString mss);

	QString parseQtFunc(QString s);
	QString toString();
};

class SLogWidget : public QWidget{
	Q_OBJECT

	QString last;
	QList<CLogE>* list;
	QTextEdit* w_edit;

public:
	SLogWidget();
	void log(ILogLevel lv, QString cl, QString m){addL(CLogE(lv, QDateTime::currentDateTime(), cl, m ));}
	void addL(CLogE e);
};
// Logs


// Configs
class SConfig : public QWidget{
	Q_OBJECT

	QJsonObject* obj;
	QJsonObject* des;
	QMap<QString, QLineEdit*>* edits;

	QLineEdit *ek, *ev, *ed;
public slots:
	void wclose();
	void add();
public:
	SConfig();
	QJsonValue get(QString k);
	bool has(QString k);
	void set(QString k, QString v);
	void save();
};
// Configs

#endif // SUTILS_H
