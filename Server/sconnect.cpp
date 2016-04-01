#include "sconnect.h"

void SIrcClient::send(QString s){ srv->sendP("PRIVMSG " + sNick + " :" + s);}

void SOwnConn::newConn(){
	QTcpSocket* client = srv->nextPendingConnection();
	tmp->append(client);

	send(client, "&GetNick&");
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

				if(curr.startsWith("Nick&")){
					QStringList l = curr.split("&");
					SClient* r = new SOwnClient(l.value(1), l.value(2), client);
					tmp->removeAll(client);
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
 tmp = new QList<QTcpSocket*>;
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

void SOwnConn::send(QTcpSocket *s, QString m){
	if (!s) { return;	}

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_4);

	out << (quint16) 0;
	out << m;
	out.device()->seek(0);
	out << (quint16) (block.size() - sizeof(quint16));

	s->write(block);
	s->flush();
}

SOwnClient::SOwnClient(QString nick, QString pass, QTcpSocket *s): sNick(nick), sPass(pass), socket(s){}

QString SOwnClient::tryRead(){
	if(!socket->isOpen()) return "";
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_5_4);

	if(in.atEnd()) return "";

	if (blockSize == 0) {
		if (socket->bytesAvailable() < (int)sizeof(quint16))	return "";
		in >> blockSize;
	}

	QString curr;
	in >> curr;

	lastMsg = curr;
	blockSize = 0;
	return curr;
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
// SOwnConn


// SIRCConn
void SIRCConn::newConn() {
	sendP("NICK " + cGet("login"));
	sendP("USER " + cGet("login") + " 0 * :" + cGet("login"));
}

void SIRCConn::readyRead() {
	QByteArray line;
	do {
		line = sock->readLine();
		if (line.size()) {
			QString l = QString::fromUtf8(line.data());
			parseRead(l.remove(l.length()-2, 2));
		}else
			break;
	}
	while ( true );
}

SIRCConn::SIRCConn(SServer* c): srv(c), cl(srv->cl){
	sock = new QTcpSocket;
	QObject::connect(sock, SIGNAL(connected()), this, SLOT(newConn()));
	QObject::connect(sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void SIRCConn::start(QString ip) {
	sock->connectToHost(ip, cGet("irc-server-port").toInt());
}

void SIRCConn::parseRead(QString r) {
	if(!(r.startsWith("PING") || r.indexOf("372 bitonshik228") != -1)) qDebug() << "Read: " << r;

	QStringList s = r.split(":");

	if(r.startsWith("PING")){
		sendP("PONG :" + s[1]);

	}else if(r.indexOf("PRIVMSG") != -1){

		r = r.remove(0, 1);
		QString mess = r.split(" :")[1];
		SClient* client = cl->get(QStringRef(&r, 0, r.indexOf("!")).toString());
		mess = mess.trimmed();
		if(client != nullptr){
			client->insertMess(mess);
		}else if(r.indexOf("Nick&") != -1){
			QStringList l = mess.split("&");
			emit newConnection(new SIrcClient(this, l.value(1), l.value(2)));
		}else{
			qDebug() << "Strange, " << mess;
		}

	}else if(r.indexOf(":+i") != -1 || r.indexOf(":+i") != -1){
		logI("Connected to IRC");

	}else if(r.indexOf("No such nick/channel") != -1){
		//":irc.ya1.ru 401 bitonshik228 hevok :No such nick/channel"
		srv->delConnection(cl->get(QStringRef(&r,
																																								r.indexOf("bitonshik228")+13,
																																								r.indexOf(":No") - r.indexOf("bitonshik228") - 14
																																								).toString()));
	}
}

void SIRCConn::sendP(QString r) {
	sock->write((r + "\r\n").toUtf8());
	if(!(r.startsWith("PONG") || r.startsWith("PRIVMSG"))) qDebug() << "Send: " << r;
}
