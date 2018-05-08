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

QString converter::toSmallDecimalPoint(QString str)
{	qDebug() << "gelen data" << str;
	if(str == "00")
		return "0";
	else if(str == "01")
		return "6";
	else if(str == "02")
		return "5";
	else if(str == "04")
		return "4";
	else if(str == "08")
		return "3";
	else if(str == "10")
		return "2";
	else if(str == "20")
		return "1";
	else if(str == "03")
		return "5,6";
	else if(str == "05")
		return "4,6";
	else if(str == "06")
		return "4,5";
	else if(str == "07")
		return "4,5,6";
	else if(str == "09")
		return "3,6";
	else if(str == "11")
		return "2,6";
	else if(str == "12")
		return "2,5";
	else if(str == "13")
		return "2,5,6";
	else if(str == "14")
		return "2,4";
	else if(str == "15")
		return "2,4,6";
	else if(str == "16")
		return "2,4,5";
	else if(str == "17")
		return "2,4,5,6";
	else if(str == "18")
		return "2,3";
	else if(str == "19")
		return "2,3,6";
	else if(str == "21")
		return "1,6";
	else if(str == "22")
		return "1,5";
	else if(str == "23")
		return "1,5,6";
	else if(str == "24")
		return "1,4";
	else if(str == "25")
		return "1,4,6";
	else if(str == "26")
		return "1,4,5";
	else if(str == "27")
		return "1,4,5,6";
	else if(str == "28")
		return "1,3";
	else if(str == "29")
		return "1,3,6";
	else if(str == "30")
		return "1,2";
	else if(str == "31")
		return "1,2,6";
	else if(str == "32")
		return "1,2,5";
	else if(str == "33")
		return "1,2,4,6";
	else if(str == "34")
		return "1,2,4";
	else if(str == "35")
		return "1,2,4,6";
	else if(str == "36")
		return "1,2,4,5";
	else if(str == "37")
		return "1,2,4,5,6";
	else if(str == "38")
		return "1,2,3";
	else if(str == "39")
		return "1,2,3,6";
	else
		return "99";
}

