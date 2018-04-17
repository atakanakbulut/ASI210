#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialcom.h"
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <QTimer>
#include <QTime>
#include <QWidget>
#include <QHBoxLayout>
#include <QPlainTextEdit>

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

	void readNewData(QByteArray data);

	void controller(const QString param);
	void testTool();
private slots:
	void on_actionAbout_ASI210_triggered();
	void on_actionChange_Background_triggered();
	void on_actionBlack_triggered();
	void on_sender_clicked();
	void delay();
	void Ledinit(QString str);
	void on_actionHow_to_use_ASI210_triggered();

	void on_actionAbout_Qt_triggered();

private:
	Ui::MainWindow *ui;
	serialcom *serialc;
	QTimer *tim;
};

#endif // MAINWINDOW_H
