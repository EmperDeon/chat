#include "swgt.h"

#define SERVER_OWN_CONFIG QString("local")

void Wgt::send(){	srv->send(mess->toPlainText());}

void Wgt::sopt(){SCONFIG->show();}

void Wgt::start(){
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
			//connect(srv, SIGNAL(updList(QStringList)), this, SLOT(updList(QStringList)));
		}else{
			logE("config don't have server-ip property");
		}
	}else{
		SCONFIG->set("server-type", SERVER_OWN_CONFIG);
	}
}

void Wgt::showL(){ SLOGGER->setVisible(!SLOGGER->isVisible());}

void Wgt::read(QString s){
	consHtml += s;
	logI(s);
	cons->setHtml(consHtml);
	QTextCursor c = cons->textCursor();
	c.movePosition(QTextCursor::End);
	cons->setTextCursor(c);

	srv->send("<div style=\"color:red;\">&lt; Nick &gt;</div>: " + s + "");
}

void Wgt::updList(QStringList l){
	onln->clear();
	onln->addItems(l);
}

Wgt::Wgt(QWidget *parent)	: QWidget(parent){
	QHBoxLayout* l = new QHBoxLayout;

	// Center
	QVBoxLayout* cl1 = new QVBoxLayout;
	QHBoxLayout* cl2 = new QHBoxLayout;

	cons = new QTextEdit;
	mess = new QTextEdit;
	QPushButton* bsend = new QPushButton("Send");
	mess->setMaximumHeight(80);
	bsend->setMinimumHeight(80);

	// Right menu
	QVBoxLayout* rl1 = new QVBoxLayout;
	QPushButton* bstrt = new QPushButton("Start server"), *bLogs = new QPushButton("Logs"), *bopt = new QPushButton("Options");
	onln = new QListWidget;
	onln->setMaximumWidth(100);

	// Layout
	connect(bsend, SIGNAL(clicked()), this, SLOT(send()));
	connect(bstrt, SIGNAL(clicked()), this, SLOT(start()));
	connect(bLogs, SIGNAL(clicked()), this, SLOT(showL()));
	connect(bopt,  SIGNAL(clicked()), this, SLOT(sopt()));

	cl2->addWidget(mess);
	cl2->addWidget(bsend);
	cl1->addWidget(SLOGGER);
	cl1->addWidget(cons);
	cl1->addLayout(cl2);

	rl1->addWidget(bstrt);
	rl1->addWidget(bLogs);
	rl1->addWidget(bopt);
	rl1->addWidget(onln);

	l->addLayout(cl1);
	l->addLayout(rl1);
	setLayout(l);
	setWindowTitle("Chat server");
}

void Wgt::closeEvent(QCloseEvent *e){
	Q_UNUSED(e);
	SCONFIG->save();
}

void Wgt::keyPressEvent(QKeyEvent *e){
	switch(e->key()){
		case Qt::Key_F11: SLOGGER->show();
	}
}


