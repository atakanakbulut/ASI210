#include "netman.h"
#include <QDebug>
netman::netman(QObject *parent) : QObject(parent)
{
	udpsocket = new QUdpSocket;
	svr = new QTcpServer(this);
	stcp = new QTcpSocket(this);

	connect(udpsocket, SIGNAL(readyRead()),this,SLOT(newData()));
	connect(svr, SIGNAL(newConnection()),SLOT(receivedData()));

	if (!svr->listen(QHostAddress::Any, 1234)){
	qDebug() << "server started";
	}
	else{
		qDebug() <<"do something in case of error";
	}
	qDebug() << "initlize edildi";
	}

bool netman::connectTCPHost(QString hostname, int port)
{
	if(stcp->isOpen())
		stcp->close();
	stcp->connectToHost(QHostAddress(hostname), port);
	if(!stcp->waitForConnected(5000)){
		return false;
		qDebug() << "not connected";
	}
	else{
		qDebug() << "connected";
		sendInfo("i am connected\n");
		return true;
	}
}

bool netman::connectHost(QString hostname, int port)
{
	if(udpsocket->isOpen())
		udpsocket->close();
	udpsocket->connectToHost(QHostAddress(hostname), port);
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
	udpsocket->write("i am connected to network");
}

void netman::newData()
{
	QString ba = udpsocket->readAll();
	qDebug() << "readed data is " << ba;
	emit newUdpData(ba);
}

void netman::tcpData()
{
	QByteArray ba = stcp->readAll();
	if(ba == "exit")
		stcp->close();
	qDebug() << ba;
	emit newTcpData(QString::fromUtf8(ba));
}

void netman::receivedData()
{
	stcp = svr->nextPendingConnection();
	if (!stcp)
		return;
	qDebug("Client connected");
	stcp->write("welcome here\n");
	//stcp->waitForReadyRead(15000);
	connect(stcp, SIGNAL(readyRead()),this, SLOT(tcpData()));
	}


bool netman::currentStatus()
{
	if(udpsocket->isOpen())
		return true;
	else false;
}

bool netman::closeConnection()
{
	if(udpsocket->isOpen())
		udpsocket->close();
	return true;
}

void netman::writeTcpData(QByteArray readyData)
{
	stcp->write(readyData);
	stcp->waitForBytesWritten(3000);
}

void netman::dataSender(QByteArray barray, QString host, int port)
{
	if(!udpsocket->isOpen())
	connectHost(host, port);
	if(barray.isEmpty() || barray.isNull())
		return;
	udpsocket->write(barray+"\n");
}



