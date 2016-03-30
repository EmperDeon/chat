#include "cwgt.h"

void Wgt::send(){ srv->sendMessage(mess->text()); mess->clear();}

Wgt::Wgt(QWidget *parent)	: QWidget(parent){
	QHBoxLayout* l = new QHBoxLayout;

	// Center
	QVBoxLayout* cl1 = new QVBoxLayout;
	QHBoxLayout* cl2 = new QHBoxLayout;

	cons = new QTextEdit;
	mess = new QLineEdit;
	QPushButton* bsend = new QPushButton("Send");
	mess->setMaximumHeight(30);
	bsend->setMinimumHeight(30);

	// Right menu
	QVBoxLayout* rl1 = new QVBoxLayout;
	QPushButton* bConn = new QPushButton("Connect"), *bopt = new QPushButton("Options");
	onln = new QListWidget;
	onln->setMaximumWidth(120);
	srv = new CClient(this);

	// Layout
	connect(bsend, SIGNAL(clicked()), this, SLOT(send()));
	connect(bConn, SIGNAL(clicked()), srv, SLOT(connectToServer()));
	connect(bopt,  SIGNAL(clicked()), CCONFIG, SLOT(show()));

	cl2->addWidget(mess);
	cl2->addWidget(bsend);
	cl1->addWidget(cons);
	cl1->addLayout(cl2);

	rl1->addWidget(bConn);
	rl1->addWidget(bopt);
	rl1->addWidget(onln);

	l->addLayout(cl1);
	l->addLayout(rl1);
	setLayout(l);
	setWindowTitle("Chat");
}

void Wgt::append(QString s){
	sCons += s + "<br/>";
	cons->setHtml(sCons);
	QTextCursor c = cons->textCursor();
	c.movePosition(QTextCursor::End);
	cons->setTextCursor(c);
}

void Wgt::updList(QStringList l){
	onln->clear();
	onln->addItems(l);
}


void Wgt::closeEvent(QCloseEvent *e){
	Q_UNUSED(e);

	srv->close();
	CLOGGER->close();
}

void Wgt::keyPressEvent(QKeyEvent *e){
	switch(e->key()){
		case Qt::Key_F11: CLOGGER->show(); break;
		case 16777220: send(); e->accept(); break;
	}
}
