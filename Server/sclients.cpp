#include "sclients.h"

#define TIME QString::number(QDateTime::currentMSecsSinceEpoch())

SClients::SClients(){
	clients = new QMap<QString, SClient*>;
	users = new QMap<QString, QString>;
}

void SClients::add(SClient *c){
	clients->insert(c->getNick(), c);
}

void SClients::add(QString n, QString r){
	users->insert(n, r);
}

void SClients::del(QString n){
	clients->remove(n);
	users->remove(n);
}

// Send
void SClients::send(QString n, QString m){
	if(clients->contains(n))
		clients->value(n)->send(m);
}

void SClients::send(QStringList n, QString m){
	for(QString i : n)
		send(i, m);
}

void SClients::send(QJsonArray a, QString m){
	for(QJsonValue i : a)
		send(i.toString(), m);
}

void SClients::sendAll(QString m){ send(getNicks(), m);}

void SClients::sendMotd(QString n){
	send(n, "Motd^Hi there !");
}

void SClients::sendConn(QString n, QString m){
	sendAll(m + "^" + TIME + "^" + findN(n));
}
// Send


// Gets
QString SClients::getColor(QString n){	return clients->value(n)->getColor();}
QString SClients::getName(QString n){ return users->contains(n) ? users->value(n) : n;}
QString SClients::tryRead(QString n){	return clients->value(n)->tryRead();}
QStringList SClients::getNicks(){ return clients->keys();}
QStringList SClients::getNames(){ return users->values();}

void SClients::setColor(QString n, QString c){	clients->value(n)->setColor(c);}

QString SClients::findN(QString n){
	return users->value(n);
}

QString SClients::findR(QString r){
	for(QString i : users->keys())
		if(users->value(i) == r)
			return i;
	return "";
}
