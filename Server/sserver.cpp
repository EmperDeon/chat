#include "sserver.h"

#define TIME QString::number(QDateTime::currentMSecsSinceEpoch())
#define SERVER_OWN_CONFIG QString("local")

// Slots
void SServer::newConnection(SClient *c){
	QString n = c->getNick();
	if(!users->contains(n)) {
		users->insert(n, c->getPass());
		cl->add(c);
		cl->sendMotd(n);
		wgt->append("<div style=\"color:#009900\">" + c->getNick() + " connected</div>");

	}else if(users->value(n) == c->getPass()){
		cl->add(c);
		cl->sendMotd(n);
		wgt->append("<div style=\"color:#009900\">" + c->getNick() + " connected</div>");

	}else{
		c->send("&WrongPass&");
		c->disconnect();

	}
}

void SServer::delConnection(SClient *c){
	wgt->append("<div style=\"color:#ff3300\">" + c->getNick() + " disconnected</div>");
	QString s = cl->getName(c->getNick());
	cl->del(c->getNick());
	c->disconnect();
	cl->sendConn(c->getNick(), "Disconnected&"+s);
	updList();
}

void SServer::tryRead(){
	for(QString c : cl->getNicks()){
		QString r = cl->tryRead(c);
		if (r != "") {
			QStringList s = r.split('&');

			if(r.startsWith("Mess&")){
				appendToWidget(c, s);
				QString m =	"Mess&" + TIME + "&" + cl->getName(c) + "&" + cl->getColor(c) + "&" + s[1];

				cl->sendAll(m);
				appendHistory(TIME, m);

			}else if(r.startsWith("GetHistory&")){
				sendHistory(c, s[1]);
				cl->sendConn(c, "Connected");

			}else if(r.startsWith("SetColor&")){
				cl->setColor(c, s[1]);

			}else if(r.startsWith("SetName&")){
				cl->add(c, s[1]);
				updList();

			}else if(r.startsWith("GetList&")){
				cl->send(c, "UpdUsers&" + cl->getNicks().join('&'));

			}else if(r.startsWith("&Disconnect&")){
				delConnection(cl->get(c));

			}else{
				logI(r);
			}
		}
	}
}
// Slots


// Functions
void SServer::startServer(){
	if(cHas("server-type")){
		QString type = cGet("server-type");
		if(type == SERVER_OWN_CONFIG){
			srv = new SOwnConn;
		}else if(type == "irc"){
			srv = new SIRCConn(this);
		}else{
			logE("server-type must be " + SERVER_OWN_CONFIG + " or irc, not " + type);
		}

		if(cHas("irc-server-ip") || cHas("loc-server-ip")){
			srv->start(type=="irc" ? cGet("irc-server-ip") : cGet("loc-server-ip"));

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

SServer::SServer(Wgt *w): wgt(w){
	cl = new SClients;

 loadJsons();
}

void SServer::executeComm(QString c){
	Q_UNUSED(c);
	//srv->send(c);
}

void SServer::close() {
	saveJsons();
}

void SServer::updList(){
	wgt->updateList(cl->getNicks());
	cl->sendAll("UpdUsers&" + cl->getNames().join('^'));
}

void SServer::loadJsons() {
	QFile f("users.json");
	if(f.exists()) {
		f.open(QFile::ReadOnly);
		users = new QJsonObject(QJsonDocument::fromJson(f.readAll()).object());
		f.close();
	}else{
		users = new QJsonObject;
	}

	names = new QJsonObject;

	f.setFileName("history.json");
	if(f.exists()) {
		f.open(QFile::ReadOnly);
		history = new QJsonObject(QJsonDocument::fromJson(f.readAll()).object());
		f.close();
	}else{
		history = new QJsonObject;
	}
}

void SServer::saveJsons() {
	QFile f("users.json");
	f.open(QFile::WriteOnly);
	f.write(QJsonDocument(*users).toJson());
	f.flush();
	f.close();

	f.setFileName("history.json");
	f.open(QFile::WriteOnly);
	f.write(QJsonDocument(*history).toJson());
	f.flush();
	f.close();
}

void SServer::appendToWidget(QString c, QStringList s) {
	wgt->append("<div style=\"color:" + cl->getColor(c) + "\">&lt;" + c + "&gt;:</div>" + s[1]);
}
// Functions


// History
void SServer::sendHistory(QString c, QString d, QString filt) {
	QString v, r, date;

	for(int i = 0 ; i < 13-d.length() ; i++) date += '0';
	date += d;

	for(QString k : history->keys()){
		if(k > date){

			v = history->value(k).toString();
			if(v.startsWith(filt) && r.length() + v.length() > 400){
				cl->send(c, "History^" + r);
				r = "";
			}
			r += v + "^";
		}
}
	if(!v.isEmpty())
		cl->send(c,"History^" + r);

}

void SServer::appendHistory(QString k, QString v) {
	history->insert(k, v.replace('&', '&'));
}
// History
