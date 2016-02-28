#include "sconnect.h"

void SOwnConn::newConn(){
	QTcpSocket* client = srv->nextPendingConnection();
	tmp->append(client);

	send(client, "^GetNick^");
}

void SOwnConn::readyRead(){
	for(QTcpSocket* client : *tmp)
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

				if(curr.startsWith("Nick^")){
					QStringList l = curr.split("^");
					SClient* r = new SOwnClient(l.value(1), client);
					emit newConnection(r);

				}else{

				}

				blockSize = 0;
			}
		}
}

SOwnConn::SOwnConn(){

}

void SOwnConn::start(QString ip){
	Q_UNUSED(ip);

	blockSize = 0;

	srv = new QTcpServer(this);

	if (!srv->listen(QHostAddress::Any, 5768)) {
		logE("Construction failed "+srv->errorString());
		return;
	}

	connect(srv, SIGNAL(newConnection()), this, SLOT(newConn()));
	logI("Server started");

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(readyRead()));
	timer->start(50);
}

//void SOwnConn::send(QString m){
//	for(QTcpSocket* s : clients->values()){
//		send(s, m);
//	}
//}

//void SOwnConn::send(QString l, QString m){
//	send(clients->value(l), m);
//}

void SOwnConn::send(QTcpSocket *s, QString m){
	if (!s) { return;	}

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_4);
	logI(m);
	out << (quint16) 0;
	out << m;
	out.device()->seek(0);
	out << (quint16) (block.size() - sizeof(quint16));

	s->write(block);
	s->flush();

	logI("Send: " + m);
}


SOwnClient::SOwnClient(QString nick, QTcpSocket *s): sNick(nick), socket(s){}

QString SOwnClient::tryRead(){
	if(socket->isOpen()) return "";
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_5_4);

	if(in.atEnd()) return "";

	if (blockSize == 0) {
		if (socket->bytesAvailable() < (int)sizeof(quint16))	return "";
		in >> blockSize;
	}

	QString curr;
	in >> curr;

	if (curr != lastMsg) {
		lastMsg = curr;
		blockSize = 0;
		return curr;
	}
	return "";
}

void SOwnClient::send(QString s){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_4);

	out << (quint16) 0;
	out << s;
	out.device()->seek(0);
	out << (quint16) (block.size() - sizeof(quint16));

	socket->write(block);
	socket->flush();
}
