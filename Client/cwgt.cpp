#include "cwgt.h"

void Wgt::send(){	srv->send(mess->toPlainText());}

Wgt::Wgt(QWidget *parent)	: QWidget(parent){
	QHBoxLayout* l = new QHBoxLayout;

	// Center
	QVBoxLayout* cl1 = new QVBoxLayout;
	QHBoxLayout* cl2 = new QHBoxLayout;

	mCons = new QMap<QString, QString>;
	mMess = new QMap<QString, QString>;
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
	srv = new CClient(this);

	// Layout
	connect(bsend, SIGNAL(clicked()), this, SLOT(send()));
	connect(bconn, SIGNAL(clicked()), srv, SLOT(connectToServer()));
	connect(bopt,  SIGNAL(clicked()), CCONFIG, SLOT(show()));

	cl2->addWidget(mess);
	cl2->addWidget(bsend);
	cl1->addWidget(srv->rooms);
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

void Wgt::append(QString r, QString s){
	if(r == room){
		sCons += s;
		cons->setHtml(sCons);
		QTextCursor c = cons->textCursor();
		c.movePosition(QTextCursor::End);
		cons->setTextCursor(c);
	}else{
		mCons->insert(r, mCons->value(r) + s);
//		rooms->setBold(r, true);
	}
}

void Wgt::updList(QStringList l){
	onln->clear();
	onln->addItems(l);
}

void Wgt::changeRoom(QString r){
	mCons->insert(room, sCons);
	mMess->insert(room, mess->toPlainText());
	room = r;
	//rooms->setBold(room, false);
	sCons = mCons->value(room);
	mess->setText(mMess->value(room));
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

