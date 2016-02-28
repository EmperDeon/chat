#ifndef CWROOM_H
#define CWROOM_H
#include <cdefines.h>

class CWRoom : public QLabel{
	Q_OBJECT

	QString name;

signals:
	void clicked(QString);

public:
	CWRoom(QString n);
	void setActive(bool f);

protected:
	virtual void mouseReleaseEvent(QMouseEvent* e);
};

class CWRooms : public QWidget{
	Q_OBJECT

	QHBoxLayout* l;
	QMap<QString, CWRoom*>* map;
	QString current = "*";
	CWRoom* add;

signals:
	void roomChanged();

public slots:
	void lClicked(QString n);
	void addNew(QString n);
	QString getCurrent(){ return current;}
	void setBold(QString n, bool b);
public:
	CWRooms();

protected:
	void changeTo(QString n);
};

#endif // CWROOM_H
