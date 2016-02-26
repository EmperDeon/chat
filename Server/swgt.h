#ifndef WGT_H
#define WGT_H
#include <sdefines.h>
#include <sconnect.h>

enum SC_STATE{};

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
	SConnect* srv;

public slots:
	void send();
	void sopt();
	void start();
	void showL();

	void read(QString s);
	void updList(QStringList l);

public:
	Wgt(QWidget *parent = 0);

protected:
	virtual void closeEvent(QCloseEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);
};

#endif // WGT_H
