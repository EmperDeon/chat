#ifndef SCONNECT_H
#define SCONNECT_H
#include <sdefines.h>

enum SC_STATE{NONE, GET_NICK, GET_MESS};


class SConnect : public QObject{
	Q_OBJECT

public:
	virtual void start(QString ip) = 0;
	virtual void send(QString m) = 0;
	virtual void send(QString l, QString m) = 0;

signals:
	void onNewConnection(QString);
	void read(QString);
};

class SOwnConn : public SConnect{
	Q_OBJECT

	QTimer* t;
	QTcpServer* srv;
	QMap<QString,  QTcpSocket *>* clients;
	QMap<QTcpSocket *, SC_STATE>* states;

	QList<QTcpSocket*>* tmpMap;

	int clientsCount = 0;

	QString lastMsg;
	quint16 blockSize;

private slots:
	void newConnection();
	void readyRead();

public:
	SOwnConn();

	void parseRead(QTcpSocket* c, QString s);
	virtual void start(QString ip);
	virtual void send(QString m);
	virtual void send(QString l, QString m);

};

class SIrcConn : public SConnect{};

#endif // SCONNECT_H
