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
	bool currentStatus();

	bool connectHost(QString hostname, int port);
	void dataSender(QByteArray barray, QString host, int port);
	bool closeConnection();
	void writeTcpData(QByteArray readyData);
	bool connectTCPHost(QString hostname, int port);
private slots:
	void newData();
	void receivedData();
	void tcpData();
private:
	QUdpSocket *udpsocket;
	QTcpServer *svr;
	QTcpSocket *stcp;
	void sendInfo(const QByteArray inf);
};

#endif // NETMAN_H
