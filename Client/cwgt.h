#ifndef WGT_H
#define WGT_H
#include <cdefines.h>
#include "cclient.h"

class CClient;

class Wgt : public QWidget{
	Q_OBJECT

	// Center
	QString sCons;
	QRect size;
	QTextEdit* cons;
	QLineEdit* mess;

	// Right menu
	QListWidget* onln;
	// Tray
	QSystemTrayIcon* tray;
	QIcon i_conn, i_mess, inconn;
	bool timer_f = true, ismess = false;
	QTimer* timer;

	// Other
	CClient* srv;

public slots:
	void send();
	void trayClick(QSystemTrayIcon::ActivationReason reason);
	void connected();
	void iconUpdate();

public:
	Wgt(QWidget *parent = 0);
	void append(QString s);
	void updList(QStringList l);

protected:
	virtual void closeEvent(QCloseEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);
	virtual void hideEvent(QHideEvent *e);
	virtual void showEvent(QShowEvent* e);
	virtual void resizeEvent(QResizeEvent* e);
};

#endif // WGT_H
