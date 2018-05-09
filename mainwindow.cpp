#include "mainwindow.h"
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
	ui->LCD_label->setFont(font);
	ui->LCD_label->setAlignment(Qt::AlignCenter);
	ui->LCD_label->setText("000000");

	ui->LCDLABEL2->setFont(font);
	ui->LCDLABEL2->setAlignment(Qt::AlignCenter);
	ui->LCDLABEL2->setText("______");

	// SIGNALS AND SLOTS
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this, SLOT(testTool()));
	timer->start(7000);
	connect(serialc, SIGNAL(newTest(QString)),this,SLOT(Ledinit(QString)));
	connect(serialc,SIGNAL(sDebug(QString)),ui->console,SLOT(appendPlainText(QString)));
	serialc->initlizer();
	application a;
	a.buttonSettings();
	connect(sock,SIGNAL(newUdpData(QString)),ui->console,SLOT(appendPlainText(QString)));

	connect(serialc, SIGNAL(speak(QString)),this, SLOT(showLCDLabel2(QString)));
	//connect(serialc, SIGNAL(speak(QString)),ui->console,SLOT(appendPlainText(QString)));
	//connect(serialc, SIGNAL(speak(QString)), this, SLOT(setToLcdLabel(QString)));
	//connect(serialc, SIGNAL(sDebug(QString)), this, SLOT(setToLcdLabel(QString)));
	connect(serialc, SIGNAL(speak(QString)),ui->custom_step_console, SLOT(appendPlainText(QString)));
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

void MainWindow::on_data_send_button_clicked()
{
	QByteArray ba = ui->send_data_lineedit->text().toLocal8Bit();
	serialc->writeReadyData(ba);
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
			QString display = addLCDpoint(newValue.toInt(), ui->LCD_label->text().toLatin1());
			ui->LCD_label->setText(display);
			return;
		}
		else{
			qDebug() << "my function working";
			ui->LCD_label->setText(addLCDpoint2(newValue,ui->LCD_label->text().toLatin1()));
			return;
		}

	}
	ui->LCD_label->setText(parsedData);
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

QString MainWindow::addLCDpoint2(QString dot, QString str)
{
	qDebug() << str << dot;
	if(str.isEmpty())
		return "";
	if(dot.count() > 3)
		return ui->LCD_label->text().toLatin1();

	QStringList list = dot.split(",");
	QString n1 = list.at(0);
	QString n2 = list.at(1);
	QString data = str.insert(n1.toInt(), dotChar);
	mystring = data.insert(n2.toInt()+1, dotChar);


//	int counter = dot.count() - (( dot.count()-1 ) / 2);
//	QStringList list = dot.split(",");
//	for(int i = 0; i<counter; i++ ){
//		QString value = list.at(0);
//		qDebug() << value << "value is" << "coutner is "<<counter;
//		QString stra = str.insert(value.toInt()+i, dotChar);
//		qDebug() << str << "str is";
//		if(!counter > i){
//			mystring = stra.insert(value.toInt() +i+1,dotChar );
//		i = i + 1;
//		}
//		else if(!counter > i){
//			mystring = stra.insert(value.toInt() +i+1,dotChar );
//		i = i +1;
//		}
//		else if(counter > i ){
//			mystring = stra.insert(value.toInt() +i+1,dotChar );
//		i = i +1;
//		}
//		else if(counter > i ){
//			mystring = stra.insert(value.toInt() +i+1,dotChar );
//		i = i +1;
//		}
//		else if(counter > i ){
//			mystring = stra.insert(value.toInt() +i+1,dotChar );
//		i = i +1;
//		}
//		else if(counter > i ){
//			mystring = stra.insert(value.toInt() +i,dotChar );
//		i = i +1;
//		}
//		else
//			return mystring;

	return mystring;
}

void MainWindow::showToLCD(QString str)
{

}

void MainWindow::showLCDLabel2(QString str)
{
	if(str.count() > 12)
		return;

	if(str.contains('\b')){
		QStringList mlist = str.split('\b');
			ui->LCDLABEL2->setText(mlist.at(0));
			return;
	}/*
	else if(str.contains("\u")){
		QString mlist = str.split("\u");
		ui->LCDLABEL2->setText(mlist.at(0));
			return;
	}*/
	else
	ui->LCDLABEL2->setText(str);

}

