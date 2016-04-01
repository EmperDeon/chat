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

	i_conn = QIcon(":/icon/icon-conn.png");
	i_mess = QIcon(":/icon/icon-mess.png");
	inconn = QIcon(":/icon/icon-connn.png");

	tray = new QSystemTrayIcon(inconn);
	tray->show();
	timer = new QTimer;
	timer->setInterval(350);

	// Layout
	connect(bsend, SIGNAL(clicked()), this, SLOT(send()));
	connect(bConn, SIGNAL(clicked()), srv, SLOT(connectToServer()));
	connect(bopt,  SIGNAL(clicked()), CCONFIG, SLOT(show()));
	connect(tray,  SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayClick(QSystemTrayIcon::ActivationReason)));
	connect(srv,   SIGNAL(connected_to()), this, SLOT(connected()));
	connect(timer, SIGNAL(timeout()), this, SLOT(iconUpdate()));

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
	timer->start();
}

void Wgt::append(QString s){
	sCons += s + "<br/>";
	cons->setHtml(sCons);
	QTextCursor c = cons->textCursor();
	c.movePosition(QTextCursor::End);
	cons->setTextCursor(c);
	if(!isVisible()) tray->setIcon(i_mess), ismess = true;
}

void Wgt::updList(QStringList l){
	onln->clear();
	onln->addItems(l);
}


void Wgt::closeEvent(QCloseEvent *e){
	Q_UNUSED(e);
	tray->hide();
	srv->close();
	CLOGGER->close();
}

void Wgt::keyPressEvent(QKeyEvent *e){
	switch(e->key()){
		case Qt::Key_F11: CLOGGER->show(); break;
		case 16777220: send(); e->accept(); break;
	}
}


void Wgt::hideEvent(QHideEvent *e){
	Q_UNUSED(e)
	hide();
}

void Wgt::showEvent(QShowEvent *e){
	Q_UNUSED(e)
	ismess = false;
	tray->setIcon(i_conn);
	e->accept();
}

void Wgt::resizeEvent(QResizeEvent *e){
	size = this->rect();
}


void Wgt::trayClick(QSystemTrayIcon::ActivationReason reason) {
	if(reason == QSystemTrayIcon::Trigger){
		if (!isVisible()){
			this->resize(size.width(), size.height());
			if (isMinimized()) showNormal();
			this->show();
			qApp->setActiveWindow(this);
		//	show();
		}else{
			this->hide();
		}
	}

}

void Wgt::connected(){
	tray->setIcon(i_conn);
	append("<b style=\"color:#009900\"> Подключен к серверу </b>");
}

void Wgt::iconUpdate(){
	if(isHidden() && ismess){
		timer_f = !timer_f;
		tray->setIcon(timer_f ? i_conn : i_mess);
	}
}
