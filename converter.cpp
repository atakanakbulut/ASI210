#include "converter.h"
#include <QDebug>
converter::converter(QObject *parent) : QObject(parent)
{

}
	// format input 0x03 return "11"
QString converter::fromHexToBinary2(QString string)
{
	bool ok;
	return string.toULongLong(&ok, 16), 5, 2, QChar('0');
}
	// format input 0x03 return "11"
QString converter::fromHexToBinary(QString string)
{
	bool ok;
	return QString::number(string.toLongLong(&ok,16),2);
}

int converter::toSmallDecimalPoint(QString str)
{	qDebug() << "gelen data" << str;
	if(str == "00")
		return 0;
	else if(str == "01")
		return 6;
	else if(str == "02")
		return 5;
	else if(str == "04")
		return 4;
	else if(str == "08")
		return 3;
	else if(str == "10")
		return 2;
	else if(str == "20")
		return 1;
	else
		return 99;
}

