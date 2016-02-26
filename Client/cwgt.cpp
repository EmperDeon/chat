#include "cwgt.h"

#define SERVER_OWN_CONFIG QString("local")

void Wgt::send(){	srv->send(mess->toPlainText());}

void Wgt::sopt(){CCONFIG->show();}

void Wgt::conn(){
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

			connect(srv, SIGNAL(read(QString, QString)), this, SLOT(readResponse(QString, QString)));
			connect(srv, SIGNAL(updList(QStringList)), this, SLOT(updList(QStringList)));
		}else{
			logE("config don't have server-ip property");
		}
	}else{
		CCONFIG->set("server-type", SERVER_OWN_CONFIG);
	}
}

void Wgt::readResponse(QString k, QString s){
	if(k == room){
		sCons += s;
		cons->setHtml(sCons);
		QTextCursor c = cons->textCursor();
		c.movePosition(QTextCursor::End);
		cons->setTextCursor(c);
	}else{
		mCons->insert(k, mCons->value(k) + s);
		rooms->setBold(k, true);
	}
}

void Wgt::updList(QStringList l){
	onln->clear();
	onln->addItems(l);
}

void Wgt::changeRoom(){
	logI("room changed to " + rooms->getCurrent());

	mCons->insert(room, sCons);
	mMess->insert(room, mess->toPlainText());
	room = rooms->getCurrent();
	//rooms->setBold(room, false);
	sCons = mCons->value(room);
	mess->setText(mMess->value(room));
}

Wgt::Wgt(QWidget *parent)	: QWidget(parent){
	QHBoxLayout* l = new QHBoxLayout;

	// Center
	QVBoxLayout* cl1 = new QVBoxLayout;
	QHBoxLayout* cl2 = new QHBoxLayout;

	mCons = new QMap<QString, QString>;
	mMess = new QMap<QString, QString>;
	rooms = new CWRoom;
	cons = new QTextEdit;
	mess = new QTextEdit;
	QPushButton* bsend = new QPushButton("Send");
	mess->setMaximumHeight(80);
	bsend->setMinimumHeight(80);

	// Right menu
	QVBoxLayout* rl1 = new QVBoxLayout;
	QPushButton* bconn = new QPushButton("Connect"), *bopt = new QPushButton("Options");
	onln = new QListWidget;
	onln->setMaximumWidth(100);

	// Layout
	connect(bsend, SIGNAL(clicked()), this, SLOT(send()));
	connect(bconn, SIGNAL(clicked()), this, SLOT(conn()));
	connect(bopt, SIGNAL(clicked()), this, SLOT(sopt()));
	connect(rooms, SIGNAL(roomChanged()), this, SLOT(changeRoom()));

	cl2->addWidget(mess);
	cl2->addWidget(bsend);
	cl1->addWidget(rooms);
	cl1->addWidget(cons);
	cl1->addLayout(cl2);

	rl1->addWidget(bconn);
	rl1->addWidget(bopt);
	rl1->addWidget(onln);

	l->addLayout(cl1);
	l->addLayout(rl1);
	setLayout(l);
	setWindowTitle("Chat");
}

void Wgt::closeEvent(QCloseEvent *e){
	Q_UNUSED(e);
	CCONFIG->save();
}

void Wgt::keyPressEvent(QKeyEvent *e){
	switch(e->key()){
		case Qt::Key_F11: CLOGGER->show();
	}
}

