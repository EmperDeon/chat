#ifndef SERVER_H
#define SERVER_H
#include <cdefines.h>

class CServer : public QObject{// virtual
	Q_OBJECT

signals:
	void read(QString, QString);
	void updList(QStringList);

public:
	virtual void connect(QString a) = 0;
	virtual void send(QString s) = 0;
// virtual
};

class CIRCServer : public CServer{};

class COwnServer : public CServer{
	Q_OBJECT

	QTcpSocket* sock;
	QString lastMsg;
	quint16 blockSize = 0;

	void parseRead(QString s);
private slots:
	void readyRead();
	void connected();

public:
	COwnServer();
	virtual void connect(QString a) override;
	virtual void send(QString s) override;
};

#endif // SERVER_H
