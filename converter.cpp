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

QByteArray converter::toSmallDecimalPoint(QByteArray str)
{	qDebug() << "gelen data" << str;

	if(str == ""){
		return "";
	}
	else if(str == "\x00")
		return "99";
	else if(str == "\b")
		return "3";
	else if(str == "\x01")
		return "6";
	else if(str == "\x02")
		return "5";
	else if(str == "\x04")
		return "4";
	else if(str == "\x08")
		return "3";
	else if(str == "\x10")
		return "2";
	else if(str == "\x20")
		return "1";
	else if(str == "\x03")
		return "5,6";
	else if(str == "\x05")
		return "4,6";
	else if(str == "\x06")
		return "4,5";
	else if(str == "\x07")
		return "4,5,6";
	else if(str == "\x09")
		return "3,6";
	else if(str == "\x11")
		return "2,6";
	else if(str == "\x12")
		return "2,5";
	else if(str == "\x13")
		return "2,5,6";
	else if(str == "\x14")
		return "2,4";
	else if(str == "\x15")
		return "2,4,6";
	else if(str == "\x16")
		return "2,4,5";
	else if(str == "\x17")
		return "2,4,5,6";
	else if(str == "\x18")
		return "2,3";
	else if(str == "\x19")
		return "2,3,6";
	else if(str == "\x21")
		return "1,6";
	else if(str == "\x22")
		return "1,5";
	else if(str == "\x23")
		return "1,5,6";
	else if(str == "\x24")
		return "1,4";
	else if(str == "\x25")
		return "1,4,6";
	else if(str == "\x26")
		return "1,4,5";
	else if(str == "\x27")
		return "1,4,5,6";
	else if(str == "\x28")
		return "1,3";
	else if(str == "\x29")
		return "1,3,6";
	else if(str == "\x30")
		return "1,2";
	else if(str == "\x31")
		return "1,2,6";
	else if(str == "\x32")
		return "1,2,5";
	else if(str == "\x33")
		return "1,2,4,6";
	else if(str == "\x34")
		return "1,2,4";
	else if(str == "\x35")
		return "1,2,4,6";
	else if(str == "\x36")
		return "1,2,4,5";
	else if(str == "\x37")
		return "1,2,4,5,6";
	else if(str == "\x38")
		return "1,2,3";
	else if(str == "\x39")
		return "1,2,3,6";
	else
		return "99";
}

