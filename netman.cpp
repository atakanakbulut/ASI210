#include "netman.h"
#include <QDebug>
netman::netman(QObject *parent) : QObject(parent)
{
	udpsocket = new QUdpSocket;
	connect(udpsocket, SIGNAL(readyRead()),this,SLOT(newData()));
}

bool netman::connectHost(QString hostname,int host)
{
	if(udpsocket->isOpen())
		udpsocket->close();
	udpsocket->connectToHost(QHostAddress(hostname), host);
	if(!udpsocket->waitForConnected(5000)){
		return false;
		qDebug() << "not connected";
	}
	else{
		qDebug() << "connected";
		sendInfo("i am connected\n");
		return true;
	}
}

void netman::sendInfo(QByteArray inf)
{
	qDebug() << "connected and writing this" << inf;
	udpsocket->write("i am connected");
}

void netman::newData()
{
	QString ba = udpsocket->readAll();
	qDebug() << "readed data is " << ba;
	emit newUdpData(ba);
}
