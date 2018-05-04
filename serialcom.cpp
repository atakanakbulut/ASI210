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

	QString array2 = "aa";
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

QByteArray serialcom::readData()
{
	QByteArray ba = serial->readAll();
	QByteArray emit speak(ba);
	return ba;
}

void serialcom::writeReadyData(QByteArray ba)
{
	qDebug() << "Written data is" << ba;
	serial->write(ba);
}

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
	emit sDebug("test-atakan");
}
