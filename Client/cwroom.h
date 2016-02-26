#ifndef CWROOM_H
#define CWROOM_H
#include <cdefines.h>

class CWLbl : public QLabel{
	Q_OBJECT

	QString name;

signals:
	void clicked(QString);

public:
	CWLbl(QString n);
	void setActive(bool f);

protected:
	virtual void mouseReleaseEvent(QMouseEvent* e);
};

class CWRoom : public QWidget{
	Q_OBJECT

	QHBoxLayout* l;
	QMap<QString, CWLbl*>* map;
	QString current = "*";
	CWLbl* add;

signals:
	void roomChanged();

public slots:
	void lClicked(QString n);
	void addNew(QString n);
	QString getCurrent(){ return current;}
	void setBold(QString n, bool b);
public:
	CWRoom();

protected:
	void changeTo(QString n);
};

#endif // CWROOM_H
