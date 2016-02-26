#include "sutils.h"

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
	r = r.insert(1, '-');
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

SLogWidget::SLogWidget() :QWidget(){
	list = new QList<CLogE>;
	last = "";

	QVBoxLayout* vlay = new QVBoxLayout();

	w_edit = new QTextEdit();

	vlay->addWidget(w_edit);

	w_edit->setStyleSheet("font: 10pt \"Fantasque Sans Mono\";");
	this->setLayout(vlay);
}

void SLogWidget::addL(CLogE e){
		last += e.toString();
		w_edit->setHtml(last);
		QTextCursor c = w_edit->textCursor();
		c.movePosition(QTextCursor::End);
		w_edit->setTextCursor(c);

	list->append(e);
}

SLogWidget* SLOGGER;
// Logger


// CConfig
void SConfig::wclose(){
	for(QString k : edits->keys()){
		obj->insert(k, edits->value(k)->text());
	}
	save();
	hide();
}

void SConfig::add(){
	obj->insert(ek->text(), ev->text());
	des->insert(ek->text(), ed->text());

	ek->clear();
	ev->clear();
	ed->clear();
}

SConfig::SConfig(){
	QFile f("config.json");
	if(f.exists()){
		f.open(QFile::ReadOnly);
		obj = new QJsonObject(QJsonDocument::fromJson(f.readAll()).object());
		f.close();
	}else{
		obj = new QJsonObject;
		logE("No config file");
	}
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

	ek = new QLineEdit("Key");
	ev = new QLineEdit("Value");
	ed = new QLineEdit("Description");
	QHBoxLayout* w = new QHBoxLayout;
	QPushButton* bSave = new QPushButton("Save and Exit"), *bAdd = new QPushButton("Add");
	w->addWidget(ek);
	w->addWidget(ev);
	w->addWidget(ed);
	l->addLayout(w);
	connect(bSave, SIGNAL(clicked()), this, SLOT(wclose()));
	connect(bAdd , SIGNAL(clicked()), this, SLOT(add()));
	l->addWidget(bSave);
	l->addWidget(bAdd);
	setLayout(l);
	setWindowTitle("Options editor");
}

QJsonValue SConfig::get(QString k){	return obj->value(k);}

bool SConfig::has(QString k){ return obj->contains(k);}

void SConfig::set(QString k, QString v){ obj->insert(k, v); }

void SConfig::save(){
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
SConfig* SCONFIG;
