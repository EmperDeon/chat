#ifndef WGT_H
#define WGT_H
#include <cdefines.h>
#include <cserver.h>
#include <cwroom.h>

class Wgt : public QWidget{
	Q_OBJECT

	// Center
	CWRoom* rooms;
	QMap<QString, QString> *mCons, *mMess;
	QString sCons, room = "*";

	QTextEdit* cons;
	QTextEdit* mess;
	// Right menu
	QListWidget* onln;
	// Tray

	// Other
	CServer* srv;

public slots:
	void send();
	void sopt();
	void conn();
	void readResponse(QString r, QString s);
	void updList(QStringList l);
	void changeRoom();
public:
	Wgt(QWidget *parent = 0);

protected:
	virtual void closeEvent(QCloseEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);
};

#endif // WGT_H
