#ifndef NETMAN_H
#define NETMAN_H

#include <QObject>
#include <QUdpSocket>


class netman : public QObject
{
	Q_OBJECT
public:
	explicit netman(QObject *parent = 0);

signals:
	void newUdpData(QString data);
public slots:

	bool connectHost(QString hostname, int host);
private slots:
	//void readData(QByteArray ba);
	void newData();
private:
	QUdpSocket *udpsocket;
	void sendInfo(const QByteArray inf);
};

#endif // NETMAN_H
