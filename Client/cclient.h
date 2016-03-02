#ifndef CCLIENT_H
#define CCLIENT_H
#include <cdefines.h>
#include <cserver.h>
#include <cwgt.h>

class CServer;
class Wgt;

class CClient : public QObject{
	Q_OBJECT

	Wgt* wgt;
	CServer* srv;

	// History
	QJsonObject *history;
	bool connected = false;

protected slots:
	void read(QString r);

public slots:
	void connectToServer();

protected:

public:
	CClient(Wgt* w);

	void sendMessage(QString s);
	void send(QString s);
	void close();
};

#endif // CCLIENT_H
