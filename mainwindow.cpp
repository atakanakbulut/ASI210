#include "mainwindow.h"
#include "ui_mainwindow.h"
		/* Define to colors */
#define reds "background-color: red"
#define blues "background-color: blue"
#define gray "background-color: gray"
#define green "background-color: green"
#define black "background-color: black"
#define yellows "background-color: yellow"
#define whites "background-color: white"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setStyleSheet(black);
	ui->groupBox->setStyleSheet(black);
	ui->lcdNumber_1->frameRect();

	ui->lcdNumber_1->setPalette(Qt::red);
	ui->lcdNumber_2->setPalette(Qt::red);
	ui->lcdNumber_3->setPalette(Qt::red);
	ui->lcdNumber_4->setPalette(Qt::red);
	ui->lcdNumber_5->setPalette(Qt::red);

	/*
	ui->groupBox->setStyleSheet(black);
*/
	serialc = new serialcom;

	bool status = serialc->openConnetions();// sopenConnetions();
	if (status) {
		ui->toolButton->setStyleSheet(green);
		ui->toolButton->setText("connected");
	} else {
		ui->toolButton->setStyleSheet(reds);
		ui->toolButton->setText("not connected");
	}

	ui->lcdNumber_1->display("A");
	ui->lcdNumber_2->display("B");
	ui->lcdNumber_3->display("C");
	ui->lcdNumber_4->display("D");
	ui->lcdNumber_5->display("E");

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
	  connect(timer, SIGNAL(timeout()), this, SLOT(testTool()));
	  timer->start(300);

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

void MainWindow::testTool(){


		for(int i=0;i<10;i++){
			ui->lcdNumber_1->display(i);
			delay();
			ui->lcdNumber_2->display(i);
			delay();
			ui->lcdNumber_3->display(i);
			delay();
			ui->lcdNumber_4->display(i);
			delay();
			ui->lcdNumber_5->display(i);
			delay();

		}

}

void MainWindow::delay()
{
	QTime dieTime= QTime::currentTime().addMSecs(10);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
