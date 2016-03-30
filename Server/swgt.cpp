#include "swgt.h"

void Wgt::send(){	srv->executeComm(mess->toPlainText());}

void Wgt::showL(){ SLOGGER->setVisible(!SLOGGER->isVisible());}

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
 srv = new SServer(this);

	// Right menu
	QVBoxLayout* rl1 = new QVBoxLayout;
	QPushButton* bstrt = new QPushButton("Start server"), *bLogs = new QPushButton("Logs"), *bopt = new QPushButton("Options");
	onln = new QListWidget;
	onln->setMaximumWidth(100);

	// Layout
	connect(bsend, SIGNAL(clicked()), this, SLOT(send()));
	connect(bstrt, SIGNAL(clicked()), srv , SLOT(startServer()));
	connect(bLogs, SIGNAL(clicked()), this, SLOT(showL()));
	connect(bopt,  SIGNAL(clicked()), SCONFIG, SLOT(show()));

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

void Wgt::append(QString s){
	consHtml += s + "<br/>";
	cons->setHtml(consHtml);
	QTextCursor c = cons->textCursor();
	c.movePosition(QTextCursor::End);
	cons->setTextCursor(c);
}

void Wgt::updateList(QStringList l){
	onln->clear();
	onln->addItems(l);
}

void Wgt::closeEvent(QCloseEvent *e){
	Q_UNUSED(e);
	srv->close();
	SCONFIG->save();
}



