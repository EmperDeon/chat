#ifndef SCONNECT_H
#define SCONNECT_H
#include <sdefines.h>

class SClient{
public:
	virtual QString tryRead() = 0;
	virtual void insertMess(QString m) = 0;

	virtual void send(QString s) = 0;

	virtual QString  getColor() = 0;
	virtual QString  getNick () = 0;
	virtual QString  getPass () = 0;

	virtual void setColor(QString s ) = 0;

 virtual void disconnect() = 0;
};

class SIrcClient : public SClient{
	QString sColor, sNick, sPass;
 QStringList messages;
 SIRCConn* srv;

public:
	SIrcClient(SIRCConn* s, QString nick, QString pass): srv(s), sNick(nick), sPass(pass){ }

	virtual QString tryRead() {return messages.isEmpty() ? "" : messages.takeFirst();}
	virtual void insertMess(QString m){ messages << m;}
	virtual void send(QString s){ srv->sendP("PRIVMSG " + sNick + " :" + s);}

	virtual QString  getColor(){ return sColor;}
	virtual QString  getNick (){ return sNick ;}
	virtual QString  getPass (){ return sPass ;}

	virtual void setColor(QString s ){ sColor = s;}

	virtual void disconnect(){}
};

class SOwnClient : public SClient{
	QString sColor, sNick, sPass;
	QTcpSocket* socket;

	QString lastMsg;
	quint16 blockSize = 0;
public:
	SOwnClient(QString nick, QString pass, QTcpSocket* s);

	virtual QString tryRead();
	virtual void insertMess(QString m){ }
	virtual void send(QString s);

	virtual QString  getColor(){ return sColor;}
	virtual QString  getNick (){ return sNick ;}
	virtual QString  getPass (){ return sPass ;}

	virtual void setColor(QString s ){ sColor = s;}

	virtual void disconnect(){ socket->disconnectFromHost();}
};
// SClient


// SConnect
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

class SIRCConn : public SConnect{
 Q_OBJECT

	QTcpSocket* sock;
	QString lastMsg;

	void parseRead(QString r);

private slots:
	void newConn();
	void readyRead();

public:
	SIRCConn();
	void sendP(QString r);

	virtual void start(QString ip);};

#endif // SCONNECT_H
