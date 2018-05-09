﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "startup.h"
#include <QDir>

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

	// CREATED NEW OBJECTS
	serialc = new serialcom;
	sock = new netman;

	// SERIAL PORT INITLIZING
	bool status = serialc->openConnetions();	// sopenConnetions();
	if (status) {
		ui->connectionstatus->setStyleSheet(green);
		ui->connectionstatus->setText("connected");
		ui->console->appendPlainText("connected to ttyUSB0");
	} else {
		ui->connectionstatus->setStyleSheet(reds);
		ui->connectionstatus->setText("not connected");
		ui->console->appendPlainText("not connected to "
									 "ttyUSB0\nPlease check Serial converter or device");
	}
	// get memory address for initlizing
	int value = 0;
	int step = 0;

	dataForASI();

	//USER INTERFACE OPTIONS
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


	oldData = "";
	QFont font("Arial", 196, QFont::Bold);

	ui->LCDLABEL2->setFont(font);
	ui->LCDLABEL2->setAlignment(Qt::AlignVCenter);
	ui->LCDLABEL2->setText("------");

	// SIGNALS AND SLOTS
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this, SLOT(testTool()));
	timer->start(7000);
	//connect(serialc,SIGNAL(sDebug(QString)),ui->console,SLOT(appendPlainText(QString)));
	serialc->initlizer();
	application a;
	a.buttonSettings();
	connect(sock,SIGNAL(newUdpData(QString)),ui->console,SLOT(appendPlainText(QString)));

	connect(serialc, SIGNAL(speak(QByteArray)),this, SLOT(showLCDLabel2(QByteArray)));

	connect(serialc, SIGNAL(speak(QByteArray)),ui->custom_step_console, SLOT(appendPlainText(QByteArray)));
	connect(serialc, SIGNAL(writtenData(QString)),ui->console,SLOT(appendPlainText(QString)));
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
}

void MainWindow::on_sender_clicked()
{
	if(!ui->comboBox->currentText().isNull() && !ui->comboBox->currentText().isEmpty()){
		QByteArray bayt = ui->comboBox->currentText().toUtf8();
		serialc->writeReadyData(bayt);
	}
}

void MainWindow::readNewData(QString data)
{
	ui->console->insertPlainText(data);
	ui->console->moveCursor(QTextCursor::Down);
	qDebug() << data << "BURAYA GELIYOR";

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
		serialc->debugging();
	}
	else{
		qDebug() << "somethins wrong";
	}
}

void MainWindow::testTool(){

	//controller("active");
	controller("datawritable");
	//controller("debug");
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

void MainWindow::on_actionExport_log_triggered()
{
	QDir *dir;
	QFile file( dir->currentPath()+"/debug.txt" );
	if ( file.open(QIODevice::ReadWrite) )
	{
		QTextStream stream( &file );
		stream << ui->console->document()->toPlainText() << endl;
		file.close();
	}
}

void MainWindow::on_actionImport_log_triggered()
{


}

void MainWindow::on_pushButton_5_clicked()
{
	QString str = ui->getcustomparam->text();
	if(str.isEmpty() || str.isNull())
		return;
	QDir *dir;
	QFile file( dir->currentPath()+"/customcommands.txt" );
	if ( file.open(QIODevice::ReadWrite) )
	{
		QTextStream stream( &file );
		stream << str << endl;
		file.close();
	}
	getCustomParam();
}

void MainWindow::getCustomParam()
{
	QDir *dir;
	QFile file(dir->currentPath()+"/customcommands.txt");
	if (file.open(QIODevice::ReadOnly))
	{
		QString str = file.readAll();
		QStringList list = str.split("/n");
		ui->comboBox_custom->addItems(list);
	}
}

void MainWindow::on_pushButton_6_clicked()
{
	QByteArray ba = ui->pushcmd->text().toUtf8();
	serialc->writeReadyData(ba);
}

void MainWindow::on_actionSet_server_adress_triggered()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Please Input server and port"),
										 tr("Server Adress:"), QLineEdit::Normal,
										 "10.50.255.225:6750", &ok);

	QStringList list = text.split(":");
	QString port = list.at(1);
	sock->connectHost(list.at(0),port.toInt());
	setValueToLCD("13");
}

void MainWindow::setValueToLCD(QString ba)
{

}

void MainWindow::dataForASI()
{
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
}

void MainWindow::dataParser(const QString data)
{
	qDebug() << "data parser func";
}

QString MainWindow::convertDisplayChar(QString str, bool LCDmode )
{
	if(str.count() == 6){
		qDebug() << "6 DIGIT";
		return str;
	}
	else if(str.count() == 4 ){
		qDebug() << "CONTROLLING FOR DATA DECIMAN POINT";
		return str;
	}
	else
		return "0";

}

void MainWindow::setToLcdLabel(QString str)
{
	qDebug() << "emmitted signal" <<str;
	QString parsedData = convertDisplayChar(str,true);
	if(parsedData == "0")
		return;
	else if(parsedData.contains("0x")){
		QStringList list = str.split("x");
		QString newValue;
		convert = new converter;
		if(!list.at(1).isNull())
			newValue = convert->toSmallDecimalPoint(list.at(1));
		qDebug() << "buraya geliyor str.at.isnot null and get int value" << value;

		if(newValue == "0")
			ui->label->setText("______");
		else if(newValue == "99")
			return;
		else if (newValue.count() == 1 ){
			qDebug() << "burayada giriyor " << value;
			return;
		}
		else{
			qDebug() << "my function working";
			return;
		}

	}
}

QString MainWindow::addLCDpoint(int dot, QString str)
{
	qDebug() << "buraya giriyor addlcdpoint";
	if (str.isEmpty() || str.isNull())
		return "";
	QString data = str.insert(dot, dotChar);
	qDebug() <<"data is" <<data;
	return data;
}

void MainWindow::showLCDLabel2(QByteArray str)
{	qDebug() << str;
	int counter = 0;

	QString mystr = QString::fromUtf8(str.constData());

	QString mystr1 = QString::fromUtf8(str);
//	qDebug() << eqs.count()<< "list count";

	qDebug() << "#####################################33"<<str.size();
//	//QString s1 = eqs.at(0);
//	QString s2 = eqs.at(1);
//	QString s3 = eqs.at(2);
//	QString s4 = eqs.at(3);
//	QString s5 = eqs.at(4);
//	QString s6 = eqs.at(5);

//	qDebug() << "s1" << s2 << s3 << s4 << s5 << s6;


	/*

	if(ch1.toLatin1() == " ")
		counter = counter +1;
	if(ch2 == " ")
		counter = counter +1;
	if(ch3 == " ")
		counter = counter +1;
	if(ch4 == " ")
		counter = counter +1;
	if(ch5 == " ")
		counter = counter +1;
	if(ch6 == " ")
		counter = counter +1;


	/*if(counter > 4)
		return;*/
/*
	qDebug() << "#####################################################" << counter;
	*/
	if(str.size() > 6)
		ui->LCDLABEL2->setText(textConverter(str.constData()));
	else {
		return;
	}
}

QString MainWindow::textConverter(QString str)
{
	str.replace("R","r");
	str.replace("a","A");
	str.replace("O","0");
	str.replace("T","t");
	str.replace("D","d");
	str.replace("B","b");
	str.replace("U","U");
	return str;
}


// PART OF CHECKSUM

u32 MainWindow::crc_chk(u8* data, u8 length) {
	int j;
	u32 reg_crc=0xFFFF;

	while( length-- ) {
		reg_crc^= *data++;
		for (j=0; j<8; j++ ) {
			reg_crc = (reg_crc & 0x01) ? ((reg_crc >> 1)^0xA001) : (reg_crc>>1);
		}
	}
	return reg_crc;
}

void MainWindow::checksumServer(QByteArray getData)
{
	u32 fcrc;
	u8 crc_low,crc_high;
	fcrc = crc_chk((u8*)getData.data(),23);
	crc_high = (fcrc)%256;
	crc_low = (fcrc)/256;
	if((crc_high == (u8)getData[23])&&(crc_low == (u8)getData[24])){
		bool communication_established = true;
		if(communication_established){
			ui->LCDLABEL2->setText(getData);
		}
	}
}

void MainWindow::checksumClient(QString rawData)
{
	u32 fcrc;
	u8 crc_low,crc_high;
/*
	s->Tx_buffer[19] = input_status[0];
	s->Tx_buffer[20] = input_status[1];
	s->Tx_buffer[21] = alarm_input;
	s->Tx_buffer[22] = home_input;
	fcrc = crc_chk((u8*)s->Tx_buffer,23);
	crc_high = (fcrc)%256;
	crc_low = (fcrc)/256;
	s->Tx_buffer[23] = crc_high;
	s->Tx_buffer[24] = crc_low;
	*/
}





















