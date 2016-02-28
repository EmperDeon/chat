#include "cclient.h"

#define SERVER_OWN_CONFIG QString("local")


void CClient::read(QString s){
	if(s.startsWith("^GetNick^")){
		send("Nick^" + cGet("username"));
	}
}

void CClient::connectToServer(){
	if(cHas("server-type")){
		QString type = cGet("server-type");
		if(type == SERVER_OWN_CONFIG){
			srv = new COwnServer;
		}else if(type == "irc"){

		}else{
			logE("server-type must be " + SERVER_OWN_CONFIG + " or irc, not " + type);
		}

		if(cHas("server-ip")){
			srv->connect(cGet("server-ip"));

			connect(srv, SIGNAL(read(QString)), this, SLOT(readResponse(QString)));
		}else{
			logE("config don't have server-ip property");
		}
	}else{
		CCONFIG->set("server-type", SERVER_OWN_CONFIG);
	}
}

CClient::CClient(Wgt *w): wgt(w){

}

void CClient::sendMessage(QString s){
	send("Mess^"+s);
}

void CClient::send(QString s){
	srv->send(s);
}
