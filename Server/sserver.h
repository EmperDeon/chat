#ifndef SSERVER_H
#define SSERVER_H
#include <sdefines.h>
#include <sclients.h>
#include <sconnect.h>
#include <swgt.h>

class Wgt;
class SConnect;
class SClients;

class SServer : public QObject{
	Q_OBJECT

	Wgt* wgt;
	SConnect* srv;
	SClients* cl;

 // Autorization
	QJsonObject* users;
 QJsonObject* names;

	// History
 QJsonObject* history;

protected slots:
	void newConnection(SClient* c);
	void delConnection(SClient* c);
	void tryRead();

public slots:
	void startServer();

public:
	SServer(Wgt* w);
	void executeComm(QString c);
 void close();

protected:
	void updList();
	void loadJsons();
	void saveJsons();

	// Messages
	void appendToWidget(QString c, QStringList s);

	// History
	void appendHistory(QString k, QString v);
	void sendHistory(QString c, QString d, QString filt = "");

};


#endif // SSERVER_H
