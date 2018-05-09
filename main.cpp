#include "mainwindow.h"
#include "startup.h"
#include <QApplication>
#include <QObject>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
/*
	startup s;
	s.show();
	while(1){
		s.delay();
		if (s.loginstatus()) {
			MainWindow w;
			s.hide();
			w.show();
			return a.exec();
		}
	}
	*/
	MainWindow m;
	m.show();
	return a.exec();
}
