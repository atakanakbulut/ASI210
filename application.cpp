#include "application.h"

application::application()
{


}

void application::buttonSettings()
{
	MenuButtons button;
	button.leftShift = "";
	qcout << button.leftShift;
	test(&button);
}

bool application::controller(QString text){

	bool controlstatus;
	return controlstatus;
}

void application::test(MenuButtons *menu){

}

QString application::getFreeMemory()
{
	QProcess p;
	p.start("awk", QStringList() << "/MemTotal/ { print $2 }" << "/proc/meminfo");
	p.waitForFinished();
	QString memory = p.readAllStandardOutput();
	QString system_info;
	system_info.append(QString("%1 of TOTAL:3").arg(memory.toLong() / 1024).arg(1024 - (memory.toLong() / 1024)).arg(1024));
	p.close();
	return system_info;
}

bool application::shutdownSystem()
{
	QProcess *p;
	p->execute("shutdown -h now");
	p->waitForFinished(10000);
	p->close();
	return 1;
	return 0;
}

bool application::rebootSystem()
{
	QProcess *p;
	p->execute("reboot");
	if(p->waitForFinished(4000))
		return 1;
	return 0;
}

QString application::getIpInfo()
{
	foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
			return address.toString();
		}
	}
}
