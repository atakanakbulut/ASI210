#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <QSerialPort>
#include <QMessageBox>
#include <QIODevice>
#include <QDebug>
class serialcom : public QObject
{
	Q_OBJECT
public:
	explicit serialcom(QObject *parent = 0);
	bool connectionState;
	void initlizer();
	void writeReadyData(QByteArray ba);
	bool openConnetions();
signals:

public slots:

	QByteArray readData();
private:
	QSerialPort *serial;
	QMessageBox *msg;
	void closeConnection();
	bool connectionStatus();

};

#endif // SERIALCOM_H
