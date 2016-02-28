#include "sserver.h"


#define SERVER_OWN_CONFIG QString("local")

SServer::SServer(Wgt *w): wgt(w){
	clients = new QList<SClient*>;


}

void SServer::newConnection(SClient *c){
	clients->append(c);
	updList();
}

void SServer::delConnection(SClient *c){
	clients->removeAll(c);
	updList();
}

void SServer::tryRead(){
	for(SClient* c : *clients){
		QString r = c->tryRead();
		if (r != "") {
			if(r.startsWith("Mess^")){
				wgt->append(r.remove(0, 5));

			}else{
				logI(r);
			}
		}
	}
}

void SServer::read(QString s){
	wgt->append(s);

}

void SServer::startServer(){
	if(cHas("server-type")){
		QString type = cGet("server-type");
		if(type == SERVER_OWN_CONFIG){
			srv = new SOwnConn;
		}else if(type == "irc"){
		//	srv = new SIrcConn;
		}else{
			logE("server-type must be " + SERVER_OWN_CONFIG + " or irc, not " + type);
		}

		if(cHas("server-ip")){
			srv->start(cGet("server-ip"));

			connect(srv, SIGNAL(read(QString)), this, SLOT(read(QString)));
			connect(srv, SIGNAL(newConnection(SClient*)), this, SLOT(newConnection(SClient*)));
			connect(srv, SIGNAL(delConnection(SClient*)), this, SLOT(delConnection(SClient*)));

			QTimer* t = new QTimer;
			connect(t, SIGNAL(timeout()), this, SLOT(tryRead()));
			t->start(50);
		}else{
			logE("config don't have server-ip property");
		}
	}else{
		SCONFIG->set("server-type", SERVER_OWN_CONFIG);
	}
}

void SServer::executeComm(QString c){

	//srv->send(c);
}

void SServer::updList(){
	QStringList r;
	for(SClient* s : *clients)
		r << s->getNick();
	wgt->updateList(r);
}
