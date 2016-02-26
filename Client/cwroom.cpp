#include "cwroom.h"

// CWRoom - CWidget Room
void CWRoom::lClicked(QString n){
	changeTo(n);
}

void CWRoom::addNew(QString n){
	if(n == "+")
		n = QInputDialog::getText(this, "Enter channel name", "Name:");
	if(n == "" || map->contains(n)){
		logW("channel already added");
	}else{
		CWLbl* l = new CWLbl(n);
		map->insert(n, l);
		this->l->addWidget(l);
		connect(l, SIGNAL(clicked(QString)), this, SLOT(lClicked(QString)));
		//changeTo(n);
	}
}

void CWRoom::setBold(QString n, bool b){
	if(n != "+"){
		map->value(n)->setText(b ? "<b>[" + n + "]</b>" : "[" + n + "]");
	}
}

CWRoom::CWRoom(){
	map = new QMap<QString, CWLbl*>;
	l = new QHBoxLayout;
	QHBoxLayout* l1 = new QHBoxLayout;
	l->setAlignment(Qt::AlignLeft);
	l1->setAlignment(Qt::AlignLeft);

	addNew("*");
	addNew("Test");
	addNew("test2");
	this->map->value(current)->setActive(true);

	add = new CWLbl("+");
	l1->addLayout(l);
	l1->addWidget(add);
	setLayout(l1);
	connect(add, SIGNAL(clicked(QString)), this, SLOT(addNew(QString)));
}

void CWRoom::changeTo(QString n){
	this->map->value(current)->setActive(false);
	this->current = n;
	this->map->value(current)->setActive(true);

	emit roomChanged();
}
// CWRoom - CWidget Room


// CWLbl - CWidget Label
CWLbl::CWLbl(QString n): QLabel(), name(n){
	this->setText(name == "+" ? name : "[ "+name+" ]");
	this->setAutoFillBackground(true);
}
void CWLbl::setActive(bool f){
	QPalette p = palette();
	p.setBrush(QPalette::Background, QBrush(f ? QColor(200, 200, 200) : QColor::fromRgbF(0.941176, 0.941176, 0.941176)));
	this->setPalette(p);
}
void CWLbl::mouseReleaseEvent(QMouseEvent *e){ if(e->button() == Qt::LeftButton) emit clicked(name);}
// CWLbl - CWidget Label
