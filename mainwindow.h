#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialcom.h"
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <QTimer>
#include <QTime>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void testTool();
private slots:
	void on_actionAbout_ASI210_triggered();

	void on_actionChange_Background_triggered();

	void on_actionBlack_triggered();

	void on_sender_clicked();

	void delay();
private:
	Ui::MainWindow *ui;
	serialcom *serialc;
	QTimer *tim;
};

#endif // MAINWINDOW_H
