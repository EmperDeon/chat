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
	logD("Connecting to " + a);
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

	logD("Send: "+s);
}

void COwnServer::disconnectFromServer(){
	sock->disconnectFromHost();
}
// COwnServer


// CIRCServer
void CIRCServer::readyRead() {
	QByteArray line;
	do {
		line = sock->readLine();
		if (line.size()) {
			parseRead(QString::fromUtf8(line.data()));
		}else
			break;
	}
	while ( true );
}

void CIRCServer::parseRead(QString r) {
	r.truncate(r.length() - 2);
	qDebug() << r;
	QStringList s = r.split(":");
	if(r.startsWith("PING")){
		sendP("PONG :" + s[1]);
	}else if(r.indexOf("PRIVMSG") != -1){
		QString mess = r.split(" :")[1];
		if(!mess.startsWith(""))
			emit read(mess);
	}else if(r.indexOf(":+i") != -1 || r.indexOf(":+x") != -1){
		emit read("&GetNick&");
	}
}

void CIRCServer::connected() {
	sendP("NICK " + cGet("login"));
 sendP("USER " + cGet("login") + " 0 * :" + cGet("login"));
}

CIRCServer::CIRCServer(Wgt* w) : wgt(w){
 sock = new QTcpSocket;
	QObject::connect(sock, SIGNAL(connected()), this, SLOT(connected()));
	QObject::connect(sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void CIRCServer::connect(QString a) {
	sock->connectToHost(a, cGet("irc-server-port").toInt());
}

void CIRCServer::send(QString s) {
 sendP("PRIVMSG " + cGet("irc-name") + " :" + s);
}

void CIRCServer::disconnectFromServer() {

}

void CIRCServer::sendP(QString s) {
	sock->write((s + "\n").toUtf8());
}
// CIRCServer
