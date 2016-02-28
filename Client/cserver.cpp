#include "cserver.h"

// COwnServer
void COwnServer::readyRead(){
	QDataStream in(sock);
	in.setVersion(QDataStream::Qt_5_4);

	if (in.atEnd())
		return;

	if (blockSize == 0) {
		if (sock->bytesAvailable() < (int)sizeof(quint16))	return;
		in >> blockSize;
	}

	QString curr;
	in >> curr;

	if (curr != lastMsg) {
		emit read(curr);
		blockSize = 0;
	}
}

void COwnServer::connected(){
	logI("Connected to server");
}

COwnServer::COwnServer(){
	sock = new QTcpSocket();
	QObject::connect(sock, SIGNAL(connected()), this, SLOT(connected()));
	QObject::connect(sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void COwnServer::connect(QString a){
	sock->connectToHost(a, 5768);
	logI("Connecting to " + a);
}

void COwnServer::send(QString s){
	if (!sock) {	return;	}

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_4);

	out << (quint16) 0;
	out << s;
	out.device()->seek(0);
	out << (quint16) (block.size() - sizeof(quint16));

	sock->write(block);
	sock->flush();

	logI("Send: "+s);
}
// COwnServer
