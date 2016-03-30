#ifndef SCLIENTS_H
#define SCLIENTS_H
#include <sdefines.h>
#include <sconnect.h>

class SClient;

class SClients{
	QMap<QString, SClient*>* clients;
	QMap<QString, QString>* users;

public:
	SClients();

	void add(SClient* c);
	void add(QString n, QString r);
	void del(QString n);
	SClient* get(QString n){ return clients->contains(n) ? clients->value(n) : nullptr;}

	// Send functions
	void send(QString n, QString m);
	void send(QStringList n, QString m);
	void send(QJsonArray a, QString m);
	void sendAll(QString m);
	void sendMotd(QString n);
	void sendConn(QString n, QString m);

	// Get functions
	QString getColor(QString n);
	QString getName(QString n);
	QString tryRead(QString n);
	QStringList getNicks();
	QStringList getNames();
 SClient* getClient(QString n);

	void setColor(QString n, QString c);

	QString findN(QString n);// Get real name from nick
	QString findR(QString r);// Get nick from real name
};


#endif // SCLIENTS_H
