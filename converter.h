#ifndef CONVERTER_H
#define CONVERTER_H
#include <QObject>

class converter : public QObject
{
	Q_OBJECT
public:
	explicit converter(QObject *parent = 0);

signals:

public slots:
	QString fromHexToBinary(QString string);
	QString toSmallDecimalPoint(QString str);
	QString fromHexToBinary2(QString string);
private slots:
};

#endif // CONVERTER_H
