﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "startup.h"

/* Define to colors */
#define reds "background-color: red"
#define blues "background-color: blue"
#define gray "background-color: gray"
#define green "background-color: green"
#define black "background-color: black"
#define yellows "background-color: yellow"
#define whites "background-color: white"

//debugger
#define qcout qDebug()

#include<QProcess>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//this->setStyleSheet(black);
	//ui->groupBox->setStyleSheet(gray);

	ui->lcdNumber_1->Filled;
	ui->lcdNumber_1->setPalette(Qt::red);
	ui->lcdNumber_2->setPalette(Qt::red);
	ui->lcdNumber_3->setPalette(Qt::red);
	ui->lcdNumber_4->setPalette(Qt::red);
	ui->lcdNumber_5->setPalette(Qt::red);

	serialc = new serialcom;

	bool status = serialc->openConnetions();// sopenConnetions();
	if (status) {
		ui->connectionstatus->setStyleSheet(green);
		ui->connectionstatus->setText("connected");
		ui->console->appendPlainText("connected to ttyUSB0");
	} else {
		ui->connectionstatus->setStyleSheet(reds);
		ui->connectionstatus->setText("not connected");
		ui->console->appendPlainText("not connected to ttyUSB0\nPlease check Serial converter or device");
	}

	QByteArray flash_data[15];
	flash_data[0] = "device.gain";
	flash_data[1] = "device.dp";
	flash_data[2] = "device.zero_point";
	flash_data[3] = "device.max_load";
	flash_data[4] = "device.cal_entry";
	flash_data[5] = "device.cal_point";
	flash_data[6] = "device.step_value";
	flash_data[7] = "device.address";
	flash_data[8] = "device.baudrate";
	flash_data[9] = "device.relay_set_point";
	flash_data[10] = "device.zero_percent";
	flash_data[11] = "device.user_id";
	flash_data[12] = "device.speed_status";
	flash_data[13] = "0";
	flash_data[14] = "device.mode";

	for (int i=0;i < 15; i++){
		ui->comboBox->addItem(flash_data[i]);
	}

	QPixmap pixmap1(":/stop.png");
	QIcon ButtonIcon1(pixmap1);
	ui->pushButton_1->setIcon(ButtonIcon1);
	ui->pushButton_1->setIconSize(pixmap1.rect().size());

	QPixmap pixmap2(":/back.png");
	QIcon ButtonIcon2(pixmap2);
	ui->pushButton_2->setIcon(ButtonIcon2);
	ui->pushButton_2->setIconSize(pixmap2.rect().size());

	QPixmap pixmap3(":/up.png");
	QIcon ButtonIcon3(pixmap3);
	ui->pushButton_3->setIcon(ButtonIcon3);
	ui->pushButton_3->setIconSize(pixmap3.rect().size());

	QPixmap pixmap4(":/rec.png");
	QIcon ButtonIcon4(pixmap4);
	ui->pushButton_4->setIcon(ButtonIcon4);
	ui->pushButton_4->setIconSize(pixmap4.rect().size());

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this, SLOT(testTool()));
	timer->start(3000);
	connect(serialc, SIGNAL(speak(QByteArray)),this,SLOT(readNewData(QByteArray)));
	connect(serialc, SIGNAL(newTest(QString)),this,SLOT(Ledinit(QString)));
	connect(serialc,SIGNAL(sDebug(QString)),ui->console,SLOT(appendPlainText(QString)));
	serialc->initlizer();
	application a;
	a.buttonSettings();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionAbout_ASI210_triggered()
{
	QMessageBox::information(this,"About","\n\nCopyrigt (C) Bilkon < www.bilkon.com.tr >\n\n");
}

void MainWindow::on_actionChange_Background_triggered()
{
	this->setStyleSheet(whites);
	ui->groupBox->setStyleSheet(whites);
	ui->pushButton_1->setStyleSheet(black);
	ui->pushButton_3->setStyleSheet(black);
	ui->pushButton_4->setStyleSheet(black);
	ui->sender->setStyleSheet(black);
	ui->lcdNumber_1->setStyleSheet(gray);
	ui->lcdNumber_2->setStyleSheet(gray);
	ui->lcdNumber_3->setStyleSheet(gray);
	ui->lcdNumber_4->setStyleSheet(gray);
	ui->lcdNumber_5->setStyleSheet(gray);
}

void MainWindow::on_actionBlack_triggered()
{
	this->setStyleSheet(black);
	ui->groupBox->setStyleSheet(black);
	ui->pushButton_1->setStyleSheet(yellows);
	ui->pushButton_2->setStyleSheet(yellows);
	ui->pushButton_3->setStyleSheet(yellows);
	ui->pushButton_4->setStyleSheet(yellows);
	ui->sender->setStyleSheet(yellows);
	ui->lcdNumber_1->setStyleSheet(gray);
	ui->lcdNumber_2->setStyleSheet(gray);
	ui->lcdNumber_3->setStyleSheet(gray);
	ui->lcdNumber_4->setStyleSheet(gray);
	ui->lcdNumber_5->setStyleSheet(gray);

	ui->lcdNumber_3->setPalette(Qt::red);
	ui->lcdNumber_4->setPalette(Qt::red);
	ui->lcdNumber_5->setPalette(Qt::red);
}

void MainWindow::on_sender_clicked()
{
	if(!ui->comboBox->currentText().isNull() && !ui->comboBox->currentText().isEmpty()){
		QByteArray bayt = ui->comboBox->currentText().toUtf8();
		serialc->writeReadyData(bayt);
	}
}

void MainWindow::readNewData(QByteArray data)
{
	ui->console->appendPlainText(data);
	ui->console->moveCursor(QTextCursor::Down);
	qDebug() << data;
}

void MainWindow::delay()
{
	QTime dieTime= QTime::currentTime().addMSecs(10);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::Ledinit(QString str)
{
	qDebug() << str;
}

void MainWindow::controller(const QString param)
{
	if( param == "active"){
		if(serialc->currentState())
			qDebug() << "current state:" << "active";
		else
			qDebug() << "current state:" << "deactive";
	}
	else if ( param == "datawritable" )
		if(serialc->byteWritable()){
				qDebug() << "data writable";
				ui->writeablestatus->setText("Writable");
				ui->writeablestatus->setStyleSheet(green);
		}
		else{
				ui->writeablestatus->setText("Not Writable");
				ui->writeablestatus->setStyleSheet(reds);
		qDebug() << "not writable";
		}
	else if ( param == "debug"){
		ui->console->appendPlainText("atakandebugifelse");
		serialc->debugging();
		}
	else{
		qDebug() << "somethins wrong";
	}
}

void MainWindow::testTool(){

	controller("active");
	controller("datawritable");
	controller("debug");
}

void MainWindow::on_actionHow_to_use_ASI210_triggered()
{
	QWidget *w = new QWidget;
	QPlainTextEdit *te = new QPlainTextEdit;
	te->setReadOnly(true);
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addWidget(te);
	te->appendPlainText(" ABOUT ASI210 \n\nFirst read ASI210 manual This program created by Atakan");
	w->setLayout(hlayout);
	w->show();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
	return QApplication::aboutQt();
}
