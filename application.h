#ifndef APPLICATION_H
#define APPLICATION_H
#include <QObject>
#include <QDebug>
#define qcout qDebug()
#include <QCoreApplication>
#include <QProcess>
#include <QNetworkInterface>
#include <iostream>

struct MenuButtons
{
	QString resetBoard;
	QString leftShift;
	QString upShift;
	QString MenuButton;
};

class application
{
public:
	application();
	void buttonSettings();
public slots:
	bool controller(QString text);
	bool rebootSystem();
	bool shutdownSystem();
	QString getIpInfo();
	QString getFreeMemory();
private:
	void test(MenuButtons *menu);
private slots:

};

#endif // APPLICATION_H
