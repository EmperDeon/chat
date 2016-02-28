#ifndef SSERVER_H
#define SSERVER_H
#include <sdefines.h>
#include <sconnect.h>
#include <swgt.h>

class Wgt;
class SConnect;

class SServer : public QObject{
	Q_OBJECT

	Wgt* wgt;
	SConnect* srv;
	QList<SClient*>* clients;

protected slots:
	void newConnection(SClient* c);
	void delConnection(SClient* c);
	void tryRead();
	void read(QString s);

public slots:
	void startServer();

public:
	SServer(Wgt* w);
	void executeComm(QString c);
	void updList();

protected:

};


#endif // SSERVER_H
