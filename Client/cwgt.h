#ifndef WGT_H
#define WGT_H
#include <cdefines.h>
#include <cclient.h>
#include <cwroom.h>

class CClient;

class Wgt : public QWidget{
	Q_OBJECT

	// Center
	QMap<QString, QString> *mCons, *mMess;
	QString sCons, room = "*";

	QTextEdit* cons;
	QTextEdit* mess;
	// Right menu
	QListWidget* onln;
	// Tray

	// Other
	CClient* srv;

public slots:
	void send();

public:
	Wgt(QWidget *parent = 0);
	void append(QString r, QString s);
	void updList(QStringList l);
	void changeRoom(QString r);

protected:
	virtual void closeEvent(QCloseEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);
};

#endif // WGT_H
