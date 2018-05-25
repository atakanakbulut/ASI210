#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serialcom.h"
#include "netman.h"
#include "converter.h"
#include "application.h"
#include "keyboard.h"

#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <QTimer>
#include <QTime>
#include <QWidget>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QInputDialog>

typedef char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

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
	bool communication_established;


public slots:

	void readNewData(QString data);
	void controller(const QString param);
	void testTool();
	void showLCDLabel2(QByteArray str);
	u32 crc_chk(u8 *data, u8 length);
	QByteArray checksumServer(QByteArray getData);
	void checksumClient(QByteArray rawData);
	QString textConverter(QString str);
	void setToLCD(QByteArray ba);

	QString getKeyText();
	void testfunc2();
private slots:
	void on_actionAbout_ASI210_triggered();
	void on_actionChange_Background_triggered();
	void on_actionBlack_triggered();
	void delay();
	void Ledinit(QString str);
	void on_actionHow_to_use_ASI210_triggered();
	void on_actionAbout_Qt_triggered();
    void on_actionExport_log_triggered();
    void getCustomParam();
	void on_actionSet_server_adress_triggered();

	QString convertDisplayChar(QString str, bool LCDmode);
	QString addLCDpoint(QString decimalpoints, QString printableData);

	void on_BUTTON1_clicked(bool checked);
	void on_BUTTON2_clicked(bool checked);
	void on_BUTTON3_clicked(bool checked);
	void on_BUTTON4_clicked(bool checked);

	QByteArray buttonSettings();
	void getIpInfo();
	void getMemInfo();
	void on_pushcmd_clicked();

	bool eventFilter(QObject *object, QEvent *event);
	QByteArray dataTrans(QString str);
	void on_flash_button_clicked();

	void timOut(int sec);

	void autoCRC(QByteArray rawData, int size);

	QByteArray modbusSettings();
	QByteArray paSettings();
	QByteArray readWrite();
	QByteArray ADsettings();
	QByteArray DPsettings();
	void calculateLCAP();
	void on_modbus_flash_clicked();

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();



	void on_modbusreadbutton_clicked();

	void modbusconsole(QByteArray str);
private:
	Ui::MainWindow *ui;

	serialcom *serialc;
	netman *sock;
	converter *convert;
	application *app;
	Keyboard *keyboard;

	QTimer *tim;
	QTimer *tim2;

	const char dotChar = '.';
	const char backslash = '\\';
	void dataForASI();
	void dataParser(const QString data);
	QByteArray MODBUSDATA;

	int value;
	int step;

};

#endif // MAINWINDOW_H
