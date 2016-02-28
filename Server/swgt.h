#ifndef WGT_H
#define WGT_H
#include <sdefines.h>
#include <sserver.h>

class SServer;

class Wgt : public QWidget{
	Q_OBJECT

	// Center
	QTextEdit* cons;
	QString consHtml;
	QTextEdit* mess;
	// Right menu
	QListWidget* onln;
	// Tray

	// Other
	SServer* srv;

public slots:
	void send();
	void showL();

public:
	Wgt(QWidget *parent = 0);

	void append(QString s);
	void updateList(QStringList l);

protected:
	virtual void closeEvent(QCloseEvent* e);
};

#endif // WGT_H
