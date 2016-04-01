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
 QSystemTrayIcon* tray;

	// Other
	SServer* srv;

public slots:
	void send();
	void showL();
	void trayClick(QSystemTrayIcon::ActivationReason reason);

public:
	Wgt(QWidget *parent = 0);

	void append(QString s);
	void updateList(QStringList l);

protected:
	virtual void closeEvent(QCloseEvent* e);
	virtual void hideEvent(QHideEvent* e);
};

#endif // WGT_H
