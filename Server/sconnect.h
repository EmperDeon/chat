#ifndef SCONNECT_H
#define SCONNECT_H
#include <sdefines.h>

enum SC_STATE{NONE, GET_NICK, GET_MESS};

class SClient{
public:
	virtual QString tryRead() = 0;
	virtual void send(QString s) = 0;

	virtual QString  getColor() = 0;
	virtual QString  getNick () = 0;
	virtual SC_STATE getState() = 0;

	virtual void setColor(QString s ) = 0;
	virtual void setState(SC_STATE s) = 0;


};

class SIrcClient : public SClient{};

class SOwnClient : public SClient{
	QString sColor, sNick;
	SC_STATE sState;
	QTcpSocket* socket;

	QString lastMsg;
	quint16 blockSize = 0;
public:
	SOwnClient(QString nick, QTcpSocket* s);

	virtual QString tryRead();
	virtual void send(QString s);

	virtual QString  getColor(){ return sColor;}
	virtual QString  getNick (){ return sNick ;}
	virtual SC_STATE getState(){ return sState;}

	virtual void setColor(QString s ){ sColor = s;}
	virtual void setState(SC_STATE s){ sState = s;}
};

class SConnect : public QObject{
	Q_OBJECT

public:
	virtual void start(QString ip) = 0;

signals:
	void newConnection(SClient* c);
	void delConnection(SClient* c);
};

class SOwnConn : public SConnect{
	Q_OBJECT

	QTcpServer* srv;
	QList<QTcpSocket*>* tmp;

	QString lastMsg;
	quint16 blockSize;

	void send(QTcpSocket* s, QString m);
private slots:
	void newConn();
	void readyRead();

public:
	SOwnConn();

	virtual void start(QString ip);
//	virtual void send(QString m);
//	virtual void send(QString l, QString m);


};

class SIrcConn : public SConnect{};

#endif // SCONNECT_H
