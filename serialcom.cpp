#include "serialcom.h"

serialcom::serialcom(QObject *parent) : QObject(parent)
{
	// initlizer
	serial = new QSerialPort;
	connectionState = false;
	msg = new QMessageBox;
	connect(serial, SIGNAL(readyRead()),this, SLOT(readData()));


}

void serialcom::initlizer()
{
	/*try {
		serial->setPortName("ttyUSB0");
		serial->open(QIODevice::ReadWrite);
		serial->setBaudRate(QSerialPort::Baud115200);
		serial->setDataBits(QSerialPort::Data8);
		serial->setParity(QSerialPort::NoParity);
		serial->setFlowControl(QSerialPort::NoFlowControl);
		serial->setStopBits(QSerialPort::OneStop);
	qDebug() << "connected";
	connectionState = true;

	} catch (...) {
		msg->setText("\n\nWarning!! Connection to serialport\n");
		msg->exec();
		connectionState = false;
	}
	connectionStatus();*/
}

bool serialcom::openConnetions()
{
	try {
		serial->setPortName("ttyUSB0");
		if(serial->open(QIODevice::ReadWrite))
			if (serial->setBaudRate(QSerialPort::Baud115200)
					&& serial->setDataBits(QSerialPort::Data8)
					&& serial->setParity(QSerialPort::NoParity)
					&& serial->setStopBits(QSerialPort::OneStop)
					&& serial->setFlowControl(QSerialPort::NoFlowControl)){
				return true;
				qDebug() << "connected";
			}
	} catch (...) {
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

QByteArray serialcom::readData()
{
	QByteArray ba = serial->readAll();
	return ba;
}

void serialcom::writeReadyData(QByteArray ba)
{
	qDebug() << "Written data is" << ba;
	serial->write(ba);
}



