#include "serialcom.h"
#include <QMessageBox>
#include <QIODevice>

serialcom::serialcom(QObject *parent) : QObject(parent)
{
	// initlizer
	serial = new QSerialPort;
	connectionState = false;
	msg = new QMessageBox;
	connect(serial, SIGNAL(readyRead()),this, SLOT(readData()));
	connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
			SLOT(handleError(QSerialPort::SerialPortError)));
}

void serialcom::initlizer()
{

	QString array2 = "PROGRAM INITLIZED";
	emit newTest(array2);
}

bool serialcom::openConnetions()
{
	try {
		serial->setPortName("ttyUSB0");
		if(serial->open(QIODevice::ReadWrite))
			if (serial->setBaudRate(QSerialPort::Baud9600)
					&& serial->setDataBits(QSerialPort::Data8)
					&& serial->setParity(QSerialPort::NoParity)
					&& serial->setStopBits(QSerialPort::OneStop)
					&& serial->setFlowControl(QSerialPort::NoFlowControl))
			{
				return true;
				qDebug() << "connected";
			}
	} catch (...) {
		qDebug() << "not connected"	<<	serial->errorString();
		return false;
	}
}

void serialcom::closeConnection()
{
	serial->close();
}

bool serialcom::connectionStatus()
{
	if(connectionState)
		return true;
	else
		return false;
}

void serialcom::readData()
{
	qDebug() << serial->readAll() << "gelen data";

			qDebug() << "Written data is" << ba;
			if(serial->write(ba)){
				QString data = QString::fromUtf8(ba);
				emit writtenData(data);
			}

	QByteArray ba = serial->readAll();
	QString str = QString::fromUtf8(ba);
	emit speak(str);
}

void serialcom::writeReadyData(QByteArray ba)
{

bool serialcom::currentState()
{
	if(serial->isOpen())
		return true;
	else
		return false;
	serial->close();
}

bool serialcom::byteWritable()
{
	if(serial->isWritable())
		return true;
	else
		return false;
}

void serialcom::handleError(QSerialPort::SerialPortError error)
{
	if (error == QSerialPort::ResourceError) {
		qDebug() << serial->errorString();
		serial->close();
	}
}

void serialcom::debugging(){
	emit sDebug("ASI210");
	emit sDebug("0x02");
}
