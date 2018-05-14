#include "mainwindow.h"
#include "startup.h"
#include <QApplication>
#include <QObject>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow m;
	m.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	m.showMaximized();
	return a.exec();
}

void QWidget::showMaximized()
{
	// ...
	setWindowState((windowState() & ~(Qt::WindowMinimized | Qt::WindowFullScreen))
				   | Qt::WindowMaximized);
	show();
}
