#ifndef CCLIENT_H
#define CCLIENT_H
#include <cdefines.h>
#include <cserver.h>
#include <cwroom.h>
#include <cwgt.h>

class CServer;
class Wgt;
class CWRooms;

class CClient : public QObject{
	Q_OBJECT

	Wgt* wgt;
	CServer* srv;

protected slots:
	void read(QString s);

public slots:
	void connectToServer();

protected:

public:
	CWRooms* rooms;

	CClient(Wgt* w);
	void sendMessage(QString s);
	void send(QString s);
};

#endif // CCLIENT_H
