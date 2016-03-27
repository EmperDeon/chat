#ifndef SERVER_H
#define SERVER_H
#include <cdefines.h>
#include "cwgt.h"

class Wgt;

class CServer : public QObject{// virtual
	Q_OBJECT

signals:
	void read(QString);

public:
	virtual void connect(QString a) = 0;
	virtual void send(QString s) = 0;
	virtual void disconnectFromServer() = 0;
// virtual
};

class CIRCServer : public CServer{
Q_OBJECT

	QTcpSocket* sock;
	QString lastMsg;
	quint16 blockSize = 0;

	Wgt* wgt;

	void parseRead(QString r);
	void sendP(QString s);

private slots:
	void readyRead();
	void connected();

public:
	CIRCServer(Wgt* w);
	virtual void connect(QString a) override;
	virtual void send(QString s) override;
	virtual void disconnectFromServer() override;
};

class COwnServer : public CServer{
	Q_OBJECT

	QTcpSocket* sock;
	QString lastMsg;
	quint16 blockSize = 0;

private slots:
	void readyRead();
	void connected();

public:
	COwnServer();
	virtual void connect(QString a) override;
	virtual void send(QString s) override;
	virtual void disconnectFromServer() override;
};

#endif // SERVER_H
