#include "mainwindow.h"
#include "startup.h"
#include <QApplication>
#include <QObject>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow m;
	m.show();
	return a.exec();
}

