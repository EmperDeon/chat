#ifndef UTILS_H
#define UTILS_H
#include <cdefines.h>

struct CLogE {
	ILogLevel lv;
	QDateTime d;
	QString t, cl, ms;

	CLogE(ILogLevel lvl, QDateTime dt, QString cls, QString mss);

	QString parseQtFunc(QString s);
	QString toString();
};

class CLogWidget : public QWidget{
	Q_OBJECT

	ILogLevel curr;
	QString last;
	QList<CLogE>* list;
	QTextEdit* w_edit;
	QLabel* w_c;

public slots:
	void switchE() {switchL(ILogLevel::ERR);}
	void switchW() {switchL(ILogLevel::WRN);}
	void switchI() {switchL(ILogLevel::INF);}
	void switchD() {switchL(ILogLevel::DBG);}

	void refresh();
public:
	CLogWidget();
	void log(ILogLevel lv, QString cl, QString m){addL(CLogE(lv, QDateTime::currentDateTime(), cl, m ));}
	void addL(CLogE e);
	void switchL(ILogLevel lv);
};
// Logs


// Configs
class CConfig : public QWidget{
	Q_OBJECT

	QJsonObject* obj;
	QJsonObject* des;
	QMap<QString, QLineEdit*>* edits;

	QLineEdit *ek, *ev, *ed;
public slots:
	void wclose();
	void add();
public:
	CConfig();
	QJsonValue get(QString k);
	bool has(QString k);
	void set(QString k, QString v);
	void save();
};
// Configs
#endif // UTILS_H
