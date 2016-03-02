#ifndef WGT_H
#define WGT_H
#include <cdefines.h>
#include <cclient.h>

class CClient;

class Wgt : public QWidget{
	Q_OBJECT

	// Center
	QString sCons;

	QTextEdit* cons;
	QLineEdit* mess;

	// Right menu
	QListWidget* onln;
	// Tray

	// Other
	CClient* srv;

public slots:
	void send();

public:
	Wgt(QWidget *parent = 0);
	void append(QString s);
	void updList(QStringList l);

protected:
	virtual void closeEvent(QCloseEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);
};

#endif // WGT_H
