#ifndef NETMAN_H
#define NETMAN_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>

class netman : public QObject
{
	Q_OBJECT
public:
	explicit netman(QObject *parent = 0);

signals:
	void newUdpData(QString data);
	void newTcpData(QString barray);
public slots:

	bool connectHost(QString hostname, int host);
private slots:
	void newData();
	void receivedData();
private:
	QUdpSocket *udpsocket;
	QTcpServer *server;
	void sendInfo(const QByteArray inf);
};

#endif // NETMAN_H
