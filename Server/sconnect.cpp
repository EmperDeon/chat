#include "sconnect.h"

void SOwnConn::newConnection(){
	QTcpSocket* client = srv->nextPendingConnection();
	connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
	clients->insert(QString::number(clientsCount), client);
	logI("connected " + QString::number(clientsCount));
	emit onNewConnection(QString::number(clientsCount));
	clientsCount++;

	// TODO: request login/pass
}

void SOwnConn::readyRead(){
	for(QTcpSocket* client : clients->values())
		if(client->isOpen()){
			QDataStream in(client);
			in.setVersion(QDataStream::Qt_5_4);

			if (in.atEnd())
				continue;

			if (blockSize == 0) {
				if (client->bytesAvailable() < (int)sizeof(quint16))	continue;
				in >> blockSize;
			}

			QString curr;
			in >> curr;

			if (curr != lastMsg) {
				lastMsg = curr;
				parseRead(lastMsg);
				blockSize = 0;
			}
		}
}

SOwnConn::SOwnConn(){

}

void SOwnConn::parseRead(QString s){
	if(state == NONE){

	}else if(state == GET_NICK){

	}else if(state == GET_MESS){	emit read(s);
	}else{

	}
}

void SOwnConn::start(QString ip){
	Q_UNUSED(ip);

	clients = new QMap<QString, QTcpSocket*>();
	blockSize = 0;

	srv = new QTcpServer(this);

	if (!srv->listen(QHostAddress::Any, 5768)) {
		logE("Construction failed "+srv->errorString());
		return;
	}

	connect(srv, SIGNAL(newConnection()), this, SLOT(newConnection()));
	logI("Server started");

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(readyRead()));
	timer->start(10);
}

void SOwnConn::send(QString m){
	for(QString s : clients->keys()){
		send(s, m);
	}
}

void SOwnConn::send(QString l, QString m){
	QTcpSocket* socket = clients->value(l);

	if (!socket) {	qDebug() << "no connect " << l; return;	}

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_4);
	logI(m);
	out << (quint16) 0;
	out << m;
	out.device()->seek(0);
	out << (quint16) (block.size() - sizeof(quint16));

	socket->write(block);
	socket->flush();

	logI("Send: " + m);
}
