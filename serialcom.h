#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <QSerialPort>
#include <QMessageBox>
#include <QIODevice>
#include <QDebug>
#include <QTimer>
class serialcom : public QObject
{
	Q_OBJECT
public:
	explicit serialcom(QObject *parent = 0);
	bool connectionState;
	void initlizer();
	void writeReadyData(QByteArray ba);
	bool openConnetions();
	bool byteWritable();
signals:
	void speak(QByteArray ba);
	void newTest(QString str);
	void sDebug(QString debugString);
	void writtenData(QString str);
	void writeReadData(QByteArray ba);
public slots:
	void readData();
	bool currentState();
	void debugging();
	void waitForByteWritten(int msec);

	void readWriteMode();
	void screenmode();
	void closeConnection();
	void destroy();
private slots:
	void handleError(QSerialPort::SerialPortError error);
	void ReadyreadDATA();
private:
	QSerialPort *serial;
	QMessageBox *msg;
	bool connectionStatus();
	QTimer *t;
};

#endif // SERIALCOM_H
