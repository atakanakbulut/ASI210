#ifndef APPLICATION_H
#define APPLICATION_H
#include <QObject>
#include <QDebug>
#define qcout qDebug()
#include <QCoreApplication>

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
private:

	void test(MenuButtons *menu);
private slots:

};

#endif // APPLICATION_H
