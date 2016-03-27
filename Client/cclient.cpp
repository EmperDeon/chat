#include "cclient.h"

#define SERVER_OWN_CONFIG QString("local")


void CClient::read(QString r){
	QStringList s = r.split('&');
	if(r.startsWith("&GetNick&")){
		connected = true;
		send("Nick&" + cGet("login") + "&" + cGet("pass"));

	}else if(r.startsWith("&WrongPass&")){
		wgt->append("<br/><div style=\"color:red\"> Wrong password </div><br/>");

	}else if(r.startsWith("Motd&")){
		wgt->append("MOTD: " + s[1] + "<br />");
		send("SetName&"  + cGet("realname"));
		send("SetColor&" + cGet("color"));
		send("GetHistory&" + history->value("lastMessage").toString("1"));

	}else if(r.startsWith("History&")){
		for(QString i : s){
			addHistory(i);
		}

	}else if(r.startsWith("Mess&")){
		wgt->append(time(s[1]) + "<b style=\"color:" + s[3] + "\">&lt;" + s[2] + "&gt;</b>: " + s[4]);

	}else if(r.startsWith("UpdUsers&")){
		wgt->updList(s[1].split('&'));

	}else if(r.startsWith("Connected&")){
		wgt->append(time(s[1]) + "<b style=\"color:#009900\">" + s[2] + " подключился</b>");

	}else if(r.startsWith("Disconnected&")){
		wgt->append(time(s[1]) + "<b style=\"color:#ff3300\">" + s[2] + " отключился</b>");

	}else{
		connected = true;
		wgt->append("Read: " + r);
	}
}

void CClient::connectToServer(){
	if(!connected){
		type = "irc";
		QString type = cGet("server-type");
		if(type == SERVER_OWN_CONFIG){
			srv = new COwnServer;
		}else if(type == "irc"){
   srv = new CIRCServer(wgt);
		}else{
			logE("server-type must be " + SERVER_OWN_CONFIG + " or irc, not " + type);
		}
//srv->connect("92.125.155.98");
		srv->connect(cGet("server-ip"));

		connect(srv, SIGNAL(read(QString)), this, SLOT(read(QString)));
	}
}

CClient::CClient(Wgt *w): wgt(w){
	history = new QJsonObject;
}

void CClient::addHistory(QString m){
	QStringList s = m.split('&');
	if(m.startsWith("History")) return;

	if(m.startsWith("Mess&")){
			wgt->append(time(s[1]) + "<b style=\"color:" + s[3] + "\">&lt;" + s[2] + "&gt;</b>: " + s[4]);

	}else if(m.startsWith("Connected&")){
			wgt->append(time(s[1]) + "<b style=\"color:#009900\">" + s[2] + " подключился</b>");

	}else if(m.startsWith("Disconnected&")){
			wgt->append(time(s[1]) + "<b style=\"color:#ff3300\">" + s[2] + " отключился</b>");

	}else{
		logD(m);
	}
}

QString CClient::time(QString s){
	QDateTime d = QDateTime::fromMSecsSinceEpoch(s.toLongLong());
	if(d.date() != lastdate){
		wgt->append("<b style=\"color:gray\">    -------- " + d.toString("dd MMMM yyyy") + " --------</b>");
		lastdate = d.date();
	}
	return d.toString("HH:mm:ss ");
}

void CClient::sendMessage(QString s) {
	send("Mess&" + s);
}

void CClient::send(QString s){
	if(!connected) connectToServer();
	srv->send(s);
}

void CClient::close(){
	if(srv) send("&Disconnect&");
}
