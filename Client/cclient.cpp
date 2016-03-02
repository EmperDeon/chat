#include "cclient.h"

#define SERVER_OWN_CONFIG QString("local")


void CClient::read(QString r){
	qDebug() << r;
	QStringList s = r.split('^');
	if(r.startsWith("^GetNick^")){
		send("Nick^" + cGet("login") + "^" + cGet("pass"));

	}else if(r.startsWith("^WrongPass^")){
		wgt->append("<br/><div style=\"color:red\"> Wrong password </div><br/>");

	}else if(r.startsWith("Motd^")){
		wgt->append("MOTD: " + s[1] + "<br/>");
		send("SetName^"  + cGet("realname"));
		send("SetColor^" + cGet("color"));
		send("GetHistory^" + history->value("lastMessage").toString("1"));

	}else if(r.startsWith("History^")){
		for(QString i : s){
			logD("History: " + i);
		}

	}else if(r.startsWith("Mess^")){
		wgt->append("<div style=\"color:" + s[3] + "\">&lt;" + s[2] + "&gt;</div>: " + s[4]);

	}else if(r.startsWith("UpdUsers^")){
		wgt->updList(s[1].split('&'));

	}else if(r.startsWith("Connected^")){
		wgt->append("<div style=\"color:#009900\">" + s[1] + " connected</div>");

	}else if(r.startsWith("Disconnected^")){
		wgt->append("<div style=\"color:#ff3300\">" + s[1] + " disconnected</div>");

	}else{

	}
}

void CClient::connectToServer(){
	if(!connected){
		QString type = cGet("server-type");
		if(type == SERVER_OWN_CONFIG){
			srv = new COwnServer;
		}else if(type == "irc"){

		}else{
			logE("server-type must be " + SERVER_OWN_CONFIG + " or irc, not " + type);
		}

		srv->connect(cGet("server-ip"));
		connected = true;
		connect(srv, SIGNAL(read(QString)), this, SLOT(read(QString)));
	}
}

CClient::CClient(Wgt *w): wgt(w){
	history = new QJsonObject;
}

void CClient::sendMessage(QString s) {
	send("Mess^" + s);
}

void CClient::send(QString s){
	if(!connected) connectToServer();
	srv->send(s);
}

void CClient::close(){
	if(srv) send("^Disconnect^");
}
