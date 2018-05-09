#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serialcom.h"
#include "netman.h"

#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <QTimer>
#include <QTime>
#include <QWidget>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QInputDialog>
#include "converter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	QString mystring;
	QString oldData;

public slots:

	void readNewData(QString data);

	void setToLcdLabel(QString str);
	void controller(const QString param);
	void testTool();
	void showToLCD(QString str);
	void showLCDLabel2(QString str);
private slots:
	void on_actionAbout_ASI210_triggered();
	void on_actionChange_Background_triggered();
	void on_actionBlack_triggered();
	void on_sender_clicked();
	void delay();
	void Ledinit(QString str);
	void on_actionHow_to_use_ASI210_triggered();
	void on_actionAbout_Qt_triggered();
    void on_actionExport_log_triggered();
    void on_actionImport_log_triggered();
    void on_pushButton_5_clicked();
    void getCustomParam();
    void on_pushButton_6_clicked();
	void setValueToLCD(QString ba);
	void on_actionSet_server_adress_triggered();
	void on_data_send_button_clicked();

	QString convertDisplayChar(QString str, bool LCDmode);
	QString addLCDpoint(int dot, QString str);

	QString addLCDpoint2(QString dot, QString str);
private:
	Ui::MainWindow *ui;

	serialcom *serialc;
	netman *sock;
	converter *convert;
	QTimer *tim;

	void dataForASI();
	void dataParser(const QString data);

	int value;
	int step;

	const char dotChar = '.';
};

#endif // MAINWINDOW_H
