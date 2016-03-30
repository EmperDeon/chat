#include "cutils.h"

// Logger
QString getLevelName(ILogLevel lv){
	switch(lv){
	case ILogLevel::ERR : return "[E]";
	case ILogLevel::WRN : return "[W]";
	case ILogLevel::INF : return "[I]";
	case ILogLevel::DBG : return "[D]";
	case ILogLevel::QT  : return "[Q]";
	}
	return "";
}

CLogE::CLogE(ILogLevel lvl, QDateTime dt, QString cls, QString mss): lv(lvl), d(dt), cl(parseQtFunc(cls)), ms(mss){}

QString CLogE::parseQtFunc(QString s) {
	if(s.indexOf("Qt") != -1){
		return s;
	}
	QString	r = s;
	r = r.replace("virtual ", "");
	int f = r.indexOf(' ');

	if(f != -1) r = QStringRef(&r, f + 1, r.length() - f).toString();
	/*if(NO_DEBUG)	*/r = QStringRef(&r, 0, r.indexOf(':')).toString();

	return r;
}

QString CLogE::toString(){
	QString r = "<div ";

	switch(lv){
		case ILogLevel::ERR : r += "style=\"color:red";    break;
		case ILogLevel::WRN : r += "style=\"color:#ff9c00";break;
		case ILogLevel::INF : r += "style=\"color:blue";   break;
		case ILogLevel::DBG : r += "style=\"color:black";  break;
		case ILogLevel::QT  : r += "style=\"color:#5CAA15";break;
	}

	r.append("\">[");
	r.append(d.toString(ELOG_DATE_FORMAT));
	r.append("]" + getLevelName(lv));
	r.append("["+cl+"]");
	r.append(": "+ms);

	r.append("</div>");
	return r;
}

CLogWidget::CLogWidget() :QWidget(){
	list = new QList<CLogE>;
	last = "";

	QVBoxLayout* vlay = new QVBoxLayout();
	QHBoxLayout* hlay = new QHBoxLayout();

	w_edit = new QTextEdit();

	QLabel* w_l = new QLabel(tr("Show log level:"));
	QPushButton* be = new QPushButton(tr("Errors"));
	QPushButton* bw = new QPushButton(tr("Warnings"));
	QPushButton* bi = new QPushButton(tr("Info"));
	QPushButton* bd = new QPushButton(tr("Debug"));

	connect(be, SIGNAL(clicked()), this, SLOT(switchE()));
	connect(bw, SIGNAL(clicked()), this, SLOT(switchW()));
	connect(bi, SIGNAL(clicked()), this, SLOT(switchI()));
	connect(bd, SIGNAL(clicked()), this, SLOT(switchD()));

	hlay->addWidget(w_l);
	hlay->addWidget(be);
	hlay->addWidget(bw);
	hlay->addWidget(bi);
	hlay->addWidget(bd);

	vlay->addLayout(hlay);
	vlay->addWidget(w_edit);

	w_edit->setStyleSheet("font: 10pt \"Fantasque Sans Mono\";");
	this->setLayout(vlay);

	switchL(ILogLevel::DBG);
}

void CLogWidget::addL(CLogE e){
	if(curr >= e.lv ||  e.lv == ILogLevel::QT){
		last += e.toString();
		w_edit->setHtml(last);
		QTextCursor c = w_edit->textCursor();
		c.movePosition(QTextCursor::End);
		w_edit->setTextCursor(c);
	}
	list->append(e);
}

void CLogWidget::switchL(ILogLevel lv){
	curr = lv;
	refresh();
}

void CLogWidget::refresh(){
	last = "";

	QTime st;
	st.start();
		for(CLogE e : *list) {
			if(curr >= e.lv ||  e.lv == ILogLevel::QT){
				last += e.toString();
			}
		}

	w_edit->setHtml(last);
	QTextCursor c = w_edit->textCursor();
	c.movePosition(QTextCursor::End);
	w_edit->setTextCursor(c);
}

CLogWidget* CLOGGER;
// Logger


// CConfig
void CConfig::wclose(){
	for(QString k : edits->keys()){
		obj->insert(k, edits->value(k)->text());
	}
	save();
	hide();
}

CConfig::CConfig(){
	QFile f("config.json");
	if(!f.exists()){
		init();
	}

	f.open(QFile::ReadOnly);
	obj = new QJsonObject(QJsonDocument::fromJson(f.readAll()).object());
	f.close();

	f.setFileName("cdescr.json");
	if(f.exists()){
		f.open(QFile::ReadOnly);
		des = new QJsonObject(QJsonDocument::fromJson(f.readAll()).object());
		f.close();
	}else{
		des = new QJsonObject;
		logE("No config file");
	}

	edits = new QMap<QString, QLineEdit*>;
	QVBoxLayout* l = new QVBoxLayout;
	for(QString s : obj->keys()){
		QHBoxLayout* w = new QHBoxLayout;
		QLineEdit* le = new QLineEdit(obj->value(s).toString());
		w->addWidget(new QLabel(des->value(s).toString() + ": "));
		w->addWidget(le);
		l->addLayout(w);
		edits->insert(s, le);
	}

	QPushButton* bSave = new QPushButton("Save and Exit");
	connect(bSave, SIGNAL(clicked()), this, SLOT(wclose()));
	l->addWidget(bSave);
	setLayout(l);
	setWindowTitle("Options editor");
}

void CConfig::init(){
	QJsonObject o;
	QString ds, df, k;
	o.insert("color", QColorDialog::getColor(Qt::black, this, "Выбери цвет для ника").name());

	k = "login";
	ds = "Логин (только латиница): ";
	o.insert(k, QInputDialog::getText(this, "Первый запуск", ds, QLineEdit::Normal, df));

	k = "pass";
	ds = "Пароль: ";
	o.insert(k, QInputDialog::getText(this, "Первый запуск", ds, QLineEdit::Normal, df));

	k = "realname";
	ds = "Имя (на русском): ";
	o.insert(k, QInputDialog::getText(this, "Первый запуск", ds, QLineEdit::Normal, df));

//	k = "loc-server-ip";
//	ds = "Адрес локального сервера: ";
//	df = "192.168.0.1";
//	o.insert(k, QInputDialog::getText(this, "Первый запуск", ds, QLineEdit::Normal, df));

	k = "server-type";
	ds = "Тип сервера: ";
	df = "irc";
	o.insert(k, QInputDialog::getText(this, "Первый запуск", ds, QLineEdit::Normal, df));

	o.insert("irc-name", "bitonshik228");
	o.insert("irc-server-ip", "irc.ya1.ru");
	o.insert("irc-server-port", "6665");

	QFile f("config.json");
	f.open(QFile::WriteOnly);
	f.write(QJsonDocument(o).toJson());
	f.flush();
	f.close();
}

QJsonValue CConfig::get(QString k){
	QFile f("config.json");
	f.open(QFile::ReadOnly);
	obj = new QJsonObject(QJsonDocument::fromJson(f.readAll()).object());
	f.close();

	return obj->value(k);
}

bool CConfig::has(QString k){ return obj->contains(k);}

void CConfig::save(){
	QFile f("config.json");
	f.open(QFile::WriteOnly);
	f.write(QJsonDocument(*obj).toJson());
	f.flush();
	f.close();
	f.setFileName("cdescr.json");
	f.open(QFile::WriteOnly);
	f.write(QJsonDocument(*des).toJson());
	f.flush();
	f.close();
}

void CConfig::closeEvent(QCloseEvent *e){ wclose(); e->accept();}

CConfig* CCONFIG;
