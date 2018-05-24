#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "startup.h"
#include <QDir>
#include<QProcess>
/* Define to colors */
#define reds "background-color: red"
#define blues "background-color: blue"
#define gray "background-color: gray"
#define green "background-color: green"
#define black "background-color: black"
#define yellows "background-color: yellow"
#define whites "background-color: white"
#include <QEvent>
//debugger
#define qcout qDebug()

struct MODE
{
	QString READDATA = "0x03"; // Read
	QString WRITEDATA = "0x06"; // write Data
};

struct ADRESS
{
	QString ADRESS1 = "ADRESS=1";
	QString ADRESS2 = "ADRESS=2";
	QString ADRESS3 = "ADRESS=3";
	QString ADRESS4 = "ADRESS=4";
	QString ADRESS5 = "ADRESS=5";
	QString ADRESS6 = "ADRESS=6";
	QString ADRESS7 = "ADRESS=7";
	QString ADRESS8 = "ADRESS=8";
};

struct PA
{
	QString PA1 = "PA=1";
	QString PA2 = "PA=2";
	QString PA4 = "PA=4";
	QString PA8 = "PA=8";
	QString PA16 = "PA=16";
	QString PA32 = "PA=32";
	QString PA64 = "PA=64";
	QString PA128 = "PA=128";
};

struct AD
{
	QString AD1 = "AD=1";
	QString AD2 = "AD=2";
	QString AD5 = "AD=5";
	QString AD10 = "AD=10";
	QString AD20 = "AD=20";
	QString AD50 = "AD=50";
};

struct DP
{
	QString DP0 = "DP=0";
	QString DP1 = "DP=1";
	QString DP2 = "DP=2";
	QString DP3 = "DP=3";
};

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
	ui->BUTTON1->setIcon(ButtonIcon1);
	ui->BUTTON1->setIconSize(pixmap1.rect().size());

	QPixmap pixmap2(":/back.png");
	QIcon ButtonIcon2(pixmap2);
	ui->BUTTON2->setIcon(ButtonIcon2);
	ui->BUTTON2->setIconSize(pixmap2.rect().size());

	QPixmap pixmap3(":/up.png");
	QIcon ButtonIcon3(pixmap3);
	ui->BUTTON3->setIcon(ButtonIcon3);
	ui->BUTTON3->setIconSize(pixmap3.rect().size());

	QPixmap pixmap4(":/rec.png");
	QIcon ButtonIcon4(pixmap4);
	ui->BUTTON4->setIcon(ButtonIcon4);
	ui->BUTTON4->setIconSize(pixmap4.rect().size());


	oldData = "";
	QFont font("Ubuntu", 209, QFont::Bold);

	ui->LCDLABEL2->setFont(font);
	//ui->LCDLABEL2->setAlignment(Qt::AlignLeft);
	ui->LCDLABEL2->setText("------");

	QTimer *timer = new QTimer(this);
	timer->start(5000);

	tim2 = new QTimer;
	serialc->initlizer();
	app = new application;
	keyboard = new Keyboard;

	communication_established = false;

	// SIGNALS AND SLOTS
	connect(timer, SIGNAL(timeout()),this, SLOT(testTool()));
	connect(sock,SIGNAL(newTcpData(QString)), ui->tcp_status, SLOT(setText(QString)));
	connect(serialc, SIGNAL(speak(QByteArray)),this, SLOT(showLCDLabel2(QByteArray)));
	connect(serialc, SIGNAL(writtenData(QString)),ui->console, SLOT(appendPlainText(QString)));
	connect(tim2, SIGNAL(timeout()), ui->console, SLOT(clear()));

	connect(serialc, SIGNAL(writeReadData(QByteArray)),this, SLOT(modbusconsole(QByteArray)));
	connect(serialc, SIGNAL(speak(QByteArray)),this, SLOT(showLCDLabel2(modbusconsole();)));

	font = ui->tabWidget->font();
	font.setPointSize(21);
	font.setBold(true);
	ui->tabWidget->setFont(font);
	tim2->start(15000);

	QTimer::singleShot(7000, this, SLOT(getIpInfo()));
	QTimer::singleShot(10000, this, SLOT(getMemInfo()));
	ui->progressbar->setStyleSheet(gray);
	ui->lcap_lineedit->installEventFilter(this);
	ui->tara_elineedit->installEventFilter(this);
	ui->calib_lineedit->installEventFilter(this);
	ui->relay_lineedit->installEventFilter(this);

	//* END OF INITLIZER **/


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
	ui->BUTTON1->setStyleSheet(black);
	ui->BUTTON3->setStyleSheet(black);
	ui->BUTTON4->setStyleSheet(black);


}
void MainWindow::on_actionBlack_triggered()
{
	this->setStyleSheet(black);
	ui->groupBox->setStyleSheet(black);
	ui->BUTTON1->setStyleSheet(yellows);
	ui->BUTTON2->setStyleSheet(yellows);
	ui->BUTTON3->setStyleSheet(yellows);
	ui->BUTTON4->setStyleSheet(yellows);
}

void MainWindow::readNewData(QString data)
{
	ui->console->insertPlainText(data);
	ui->console->moveCursor(QTextCursor::Down);
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

void MainWindow::getCustomParam()
{
	QDir *dir;
	QFile file(dir->currentPath() + "/customcommands.txt" );
	if (file.open(QIODevice::ReadOnly))
	{
		QString str = file.readAll();
		QStringList list = str.split("/n");
	}
}

void MainWindow::on_pushButton_6_clicked()
{
	QByteArray ba = ui->pushcmd->text().toUtf8();
	checksumClient(ba);
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
	//setValueToLCD("13");
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

QString MainWindow::addLCDpoint(QString decimalpoints, QString printableData)
{
	if (printableData.isEmpty() || printableData.isNull() || decimalpoints.isEmpty() || decimalpoints == "99"){
		qDebug() << "NULL YADA 99 buraya da gelior";
		return printableData;
	}
	QStringList seperateData = printableData.split(",");
	if(decimalpoints.count() == 1){
		QString dotData = printableData.insert(decimalpoints.toInt(), dotChar);
		return dotData;
	}
	else if (decimalpoints.count() == 3) {
		qDebug() << "buraya giriyor" << seperateData;
		QString d1 = decimalpoints.at(0);
		QString d2 = decimalpoints.at(2);
		qDebug() << " d1 d2 " << d1 << d2;
		QString data1 = printableData.left(6).insert(d1.toInt(), dotChar);
		QString data2 = data1.insert(d2.toInt() + 1, dotChar);
		qDebug() << "buraya da gelior";
		return data2;
	}
	else
		return "0";
}

void MainWindow::showLCDLabel2(QByteArray str)
{
	if(str.size() < 8)
		return;

	QByteArray checksum = checksumServer(str);
	if(QString::fromUtf8(checksum) == "0" )
		return;
	QByteArray parsedData = str.mid(6,1);
	QByteArray newData = convert->toSmallDecimalPoint(parsedData);
	QString mystr1 = addLCDpoint(QString::fromUtf8(newData), checksum.left(6));
	if (!checksum.isNull() || !checksum.isEmpty() ){
		if(mystr1 == "0"){
			ui->LCDLABEL2->setText(" "+checksum.left(6));
		}
		ui->LCDLABEL2->setText(" "+mystr1);
	}
}

void MainWindow::setToLCD(QByteArray ba)
{
	if(ba.size() > 6 ){
		QString display = QString::fromUtf8(ba);
	}
	else
		qDebug() << ba.size();
}

QString MainWindow::textConverter(QString str) // This function changing characters for LCD units
{																						// just call this func when need to changing character;
	str.replace("R","r");
	str.replace("a","A");
	str.replace("O","0");
	str.replace("T"," ʈ");
	str.replace("D","d");
	str.replace("B","b");
	str.replace("U","U");
	str.replace("=", ":");
	str.replace("Z","2");
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

QByteArray MainWindow::checksumServer(QByteArray getData)
{
	u32 fcrc;
	u8 crc_low,crc_high;
	fcrc = crc_chk((u8*)getData.data(),7);
	crc_high = (fcrc)%256;
	crc_low = (fcrc)/256;
	qDebug() << "crc_high, crc_low" <<crc_high << crc_low<< getData[7]<<getData[8];
	if((crc_high == (u8)getData[7])&&(crc_low == (u8)getData[8])){
		communication_established = true;
		if(communication_established){
			readNewData(getData + "\n" + "######## CRC DATA ESTABLİSHED ########");
			qDebug() << "###########CRC DATA ESTABLİSHED#############";
			return getData;
		}
	}
	else
		readNewData(getData + "\n" + "######## CRC NOT ESTABLİSHED ########");
	qDebug() << getData << "NOT EQUALLL";
	return "0";
}

void MainWindow::checksumClient(QByteArray rawData) // this function using for
{																																						// ssh connection
	u32 fcrc;
	u8 crc_low,crc_high;
	fcrc = crc_chk((u8*)rawData.constData(), 7);
	crc_high = (fcrc)%256;
	crc_low = (fcrc)/256;
	rawData[7] = crc_high;
	rawData[8] = crc_low;
	ui->custom_step_console->appendPlainText(rawData);
	qDebug() << "ba valueee " <<rawData;
	ui->custom_step_console->moveCursor(QTextCursor::Down);
	serialc->writeReadyData(rawData);
	serialc->waitForByteWritten(10);
}

void MainWindow::on_BUTTON1_clicked(bool checked)
{
	QByteArray BTN1(8, 0);
	BTN1 = buttonSettings();
	BTN1[6] = 0x31;
	checksumClient(BTN1);
}

void MainWindow::on_BUTTON2_clicked(bool checked)
{
	QByteArray BTN2(8, 0);
	BTN2 = buttonSettings();
	BTN2[6] = 0x32;
	checksumClient(BTN2);
}

void MainWindow::on_BUTTON3_clicked(bool checked)
{
	QByteArray BTN3(8, 0);
	BTN3 = buttonSettings();
	BTN3[6] = 0x33;
	checksumClient(BTN3);
}

void MainWindow::on_BUTTON4_clicked(bool checked)
{
	QByteArray BTN4(8, 0);
	BTN4 = buttonSettings();
	BTN4[6] = 0x34;
	checksumClient(BTN4);
}

QByteArray MainWindow::buttonSettings()
{
	QByteArray ba(8, 0); // array length 4, filled with 0
	ba[0] = 0x42; // B
	ba[1] = 0x54; // T
	ba[2] = 0x54; // T
	ba[3] = 0x4e; // N
	ba[4] = 0x30; // 0
	ba[5] = 0x30; //0
	return ba;
}

void MainWindow::getIpInfo()
{
	ui->ip_label->setText("IP ADRESS " +app->getIpInfo()+ "     ");
}

void MainWindow::getMemInfo()
{
	ui->mem_label->setText(app->getFreeMemory() + "     ");
}

void MainWindow::on_pushcmd_clicked()
{
	QByteArray ba = ui->lineEdit->text().toUtf8();
	checksumClient(ba);
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
	if(object == ui->lcap_lineedit && event->type() == QEvent::MouseButtonPress) {
		keyboard->exec();
		ui->lcap_lineedit->setText(keyboard->getKeyboardText());
		return false; // lets the event continue to the edit
	}
	else if(object == ui->calib_lineedit && event->type() == QEvent::MouseButtonPress){
		keyboard->exec();
		ui->calib_lineedit->setText(keyboard->getKeyboardText());
		return false; // lets the event continue to the edit
	}
	else if(object == ui->relay_lineedit && event->type() == QEvent::MouseButtonPress)
	{
		keyboard->exec();
		ui->relay_lineedit->setText(keyboard->getKeyboardText());
		return false; // lets the event continue to the edit
	}
	else if(object == ui->tara_elineedit && event->type() == QEvent::MouseButtonPress)
	{
		keyboard->exec();
		ui->tara_elineedit->setText(keyboard->getKeyboardText());
		return false; // lets the event continue to the edit
	}

	return false;
}

QByteArray MainWindow::dataTrans(QString str)
{
	if(ui->dpinfo->currentText().toLatin1() == "SRANG=0"){
		return str.toUtf8();
	}
	else if(ui->dpinfo->currentText().toLatin1() == "SRANG=1"){
		QString mystr2 = str.insert(5, dotChar);
		return mystr2.toUtf8();
	}else if(ui->dpinfo->currentText().toLatin1() == "SRANG=2"){
		QString mystr3 = str.insert(4, dotChar);
		return mystr3.toUtf8();
	}
	else if(ui->dpinfo->currentText().toLatin1() == "SRANG=3"){
		QString mystr4 = str.insert(3, dotChar);
		return mystr4.toUtf8();
	}
}

QString MainWindow::getKeyText(){
	Keyboard k;
	k.exec();
	QString s = k.getKeyboardText();
	return s;
}

void MainWindow::on_flash_button_clicked()
{
	if(ui->radioButton_selected->isChecked()){
		if(ui->baudrate_combobox->currentText().toLatin1() == "BAUDRATE" ||
				ui->scgs_combobox->currentText().toLatin1() == "S.C.G.S" ||
				ui->asi210adress_combobox->currentText().toLatin1() == "ASI210 COMMINICATION ADR" ||
				ui->displayval_combobox->currentText().toLatin1()  == "DISPLAY VALUE STEP"){
			return;}
		else if(ui->lcap_lineedit->text().isEmpty() ||
				ui->calib_lineedit->text().isEmpty() ||
				ui->relay_lineedit->text().isEmpty() ||
				ui->tara_elineedit->text().isEmpty() ){
			ui->status->setText("STATUS :null or non changed character");
			return;}
		else if (ui->calib_lineedit->text().count() < 6 ||
				 ui->lcap_lineedit->text().count() < 6 ||
				 ui->tara_elineedit->text().count() < 6 ||
				 ui->relay_lineedit->text().count() < 6) {
			ui->status->setText("STATUS: missed characiter please input 6 char");
			return;
		}
		// flashing combobox
		ui->status->setText("ASI210 Flashing..");
		checksumClient(ui->baudrate_combobox->currentText().toUtf8());
		timOut(2);
		ui->progressbar->setValue(12);
		checksumClient(ui->scgs_combobox->currentText().toUtf8());
		timOut(1);
		ui->progressbar->setValue(19);
		checksumClient(ui->asi210adress_combobox->currentText().toUtf8());
		timOut(2);
		ui->status->setText("ASI210 Flashing.....");
		ui->progressbar->setValue(27);
		checksumClient(ui->displayval_combobox->currentText().toUtf8());
		timOut(3);
		ui->progressbar->setValue(43);
		ui->status->setText("ASI210 Flashing.........");
		//flashing lineedits
		checksumClient(ui->dpinfo->currentText().toUtf8());		timOut(3);
		ui->progressbar->setValue(56);
		ui->status->setText("ASI210 Flashing dp .........................");
		checksumClient(dataTrans(ui->lcap_lineedit->text().toLatin1()));
		timOut(2);
		ui->progressbar->setValue(65);
		checksumClient(dataTrans(ui->calib_lineedit->text().toLatin1()));
		timOut(3);
		ui->progressbar->setValue(76);
		checksumClient(dataTrans(ui->relay_lineedit->text().toLatin1()));
		timOut(3);
		ui->progressbar->setValue(87);
		checksumClient(dataTrans(ui->tara_elineedit->text().toLatin1()));
		timOut(3);
		ui->progressbar->setValue(100);
		ui->status->setText("STATUS: Flashing complate");
	}
	else if (ui->radioButton_default->isChecked()){
		ui->status->setText("STATUS: ASI210 flashing via default parameters");
	}
	else
		ui->status->setText("please check flash mode");
}

void MainWindow::timOut(int sec)
{
	QTime dieTime= QTime::currentTime().addSecs(sec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_pushButton_clicked()
{
	//QByteArray DATA(7,0);/*
	/*
	QByteArray DATA;
	QByteArray str0 = ui->modbus_adress_combo->text().toUtf8();
	QByteArray str1 = ui->function->text().toUtf8();
	QByteArray str2 = ui->startmsb->text().toUtf8();
	QByteArray str3 = ui->startlsb->text().toUtf8();
	QByteArray str4 = ui->readmsb->text().toUtf8();
	QByteArray str5 = ui->readlsb->text().toUtf8();
	DATA[0] = str0.toInt();
	DATA[1] = str1.toInt();
	DATA[2] =  str2.toInt();
	DATA[3] =  str3.toInt();
	DATA[4] =  str4.toInt();
	DATA[5] =  str5.toInt();
	QByteArray bay = ui->function->text().toUtf8();
	int count = DATA.size();
	autoCRC(DATA, count);
*/
	/*
	QByteArray bayt;
	bayt.append(ui->modbus_adress_combo->text().toStdString().c_str());
	bayt.append(ui->function->text().toStdString().c_str());
	bayt.append(ui->startmsb->text().toStdString().c_str());
	bayt.append(ui->startlsb->text().toStdString().c_str());
	bayt.append(ui->readmsb->text().toStdString().c_str());
	bayt.append(ui->readlsb->text().toStdString().c_str());
	autoCRC(bayt, bayt.size());
	qDebug() << "CONST CHAR " << bayt << bayt.size();
	qDebug() << "data: " <<bay.toHex() <<"datasize:"<<bay.size();
	ui->modbus_textedit->appendPlainText(DATA);*/
}

void MainWindow::autoCRC(QByteArray rawData,int size) // CHECKSUM AUTO
{
	const int counter = rawData.size();
	u32 fcrc;
	u8 crc_low,crc_high;
	fcrc = crc_chk((u8*)rawData.constData(), size +1 );
	crc_high = (fcrc)%256;
	crc_low = (fcrc)/256;
	rawData[(size + 1) ] = crc_high;
	rawData[(size + 2) ] = crc_low;
	ui->modbus_textedit->appendPlainText(QString::fromUtf8(rawData));
	qDebug() << "RAWDATAA" << rawData << rawData.size() << counter;
	serialc->writeReadyData(rawData);
	serialc->waitForByteWritten(10);
}

void MainWindow::testfunc2()
{/*
	QByteArray ba;
	ba[0] = 0x03; // address1
	ba[1] = 0x06; // function yazma // 0x03 okuma
	ba[2] = 0x00; // mr1
	ba[3] = 0x07; // mr2
	ba[4] = 0x00; // mr3
	ba[5] = 0x40; // mr4
	autoCRC(ba,5);
	*/

}

void MainWindow::on_pushButton_2_clicked()
{
	/*
modbusSettings(); // adress
readWrite(); // read or write check*/
	MODBUSDATA[2] = 0x00;
	MODBUSDATA[4] = 0x00;
	calculateLCAP();
}

QByteArray MainWindow::modbusSettings() // GET ADRESS INFO  // dogru
{
	ADRESS ADR;
	QString string = ui->modbusadress->currentText().toLatin1();
	if(ADR.ADRESS1 == string){
		qDebug() <<" adress1";
		MODBUSDATA[0] = 0x01;}
	else if(ADR.ADRESS2 == string){
		qDebug() << "adress2";
		MODBUSDATA[0] = 0x02;}
	else if(ADR.ADRESS3 == string){
		qDebug() << "adress3";
		MODBUSDATA[0] = 0x03;}
	else if(ADR.ADRESS4 == string){
		qDebug() << "adress4";
		MODBUSDATA[0] = 0x04;}
	else if(ADR.ADRESS5 == string){
		qDebug() << "adress5";
		MODBUSDATA[0] = 0x05;}
	else if(ADR.ADRESS6 == string){
		qDebug() << "adress6";
		MODBUSDATA[0] = 0x06;}
	else if(ADR.ADRESS7 == string){
		qDebug() << "adress7";
		MODBUSDATA[0] = 0x07;}
	else if(ADR.ADRESS8 == string){
		qDebug() << "adress8";
		MODBUSDATA[0] = 0x08;}
	else
		qDebug() << "adress hatalı veya vb";
	return MODBUSDATA;
}

QByteArray MainWindow::readWrite() // dogru;
{
	QString str = ui->modbusreadwrite->currentText().toLatin1();
	if(str == "Write[0x06]"){
		MODBUSDATA[1] = 0x06;
		qDebug() << "write modd";
	}else if(str == "Read[0x03]"){
		MODBUSDATA[1] = 0x03;
		qDebug() << " readmode";
	}else
		MODBUSDATA[1] = 0x00;
	return MODBUSDATA;
}

QByteArray MainWindow::paSettings() // PA SETTINGS THIS ASSIGN
{
	PA P;
	MODBUSDATA[3] = 0x07;
	QString paData = ui->modbuspa->currentText().toLatin1();
	if(paData == P.PA1){
		qDebug() << "01 secildi";
		MODBUSDATA[5] = 0x01;}
	else if (paData == P.PA2){
		qDebug() << "2 secildi";
		MODBUSDATA[5] = 0x02;}
	else if (paData == P.PA4){
		qDebug() << "4 secildi";
		MODBUSDATA[5] = 0x04;}
	else if (paData == P.PA8){
		qDebug() << "8 secildi";
		MODBUSDATA[5] = 0x08;}
	else if (paData == P.PA16){
		qDebug() << "16 secildi";
		MODBUSDATA[5] = 0x10;}
	else if (paData == P.PA32){
		qDebug() << "32 secildi";
		MODBUSDATA[5] = 0x20;}
	else if (paData == P.PA64){
		qDebug() << "64 secildi";
		MODBUSDATA[5] = 0x40;}
	else if (paData == P.PA128){
		qDebug() << "128 secildi";
		MODBUSDATA[5] = 0x80;}

	qDebug() << MODBUSDATA[5];
	return MODBUSDATA;
}

QByteArray MainWindow::ADsettings()
{
	AD a;
	MODBUSDATA[3] = 0x08;
	QString adData = ui->modbusad->currentText().toLatin1();
	if(a.AD1 == adData){
		qDebug() << "ad1 selected";
		MODBUSDATA[5] = 0x01;}
	else if(a.AD2 == adData){
		qDebug() << "ad2 selected";
		MODBUSDATA[5] = 0x02;}
	else if(a.AD5 == adData){
		qDebug() << "ad5 selected";
		MODBUSDATA[5] = 0x05;}
	else if(a.AD10 == adData){
		qDebug() << "ad10 selected";
		MODBUSDATA[5] = 0xA;}
	else if(a.AD20 == adData){
		qDebug() << "ad20 selected";
		MODBUSDATA[5] = 0x14;}
	else if(a.AD50 == adData){
		qDebug() << "ad50 selected";
		MODBUSDATA[5] = 0x32;}
	else
		qDebug() << "AD FUNC not equal";
	return MODBUSDATA;
}

QByteArray MainWindow::DPsettings()
{
	DP d;
	MODBUSDATA[3] = 0x06;
	QString dpData = ui->modbusdp->currentText().toLatin1();
	if(d.DP0 == dpData){
		qDebug() << "dp0 selected";
		MODBUSDATA[5] = 0x00;}
	else if(d.DP1 == dpData){
		qDebug() << "dp1 selected";
		MODBUSDATA[5] = 0x01;}
	else if(d.DP2 == dpData){
		qDebug() << "dp2 selected";
		MODBUSDATA[5] = 0x02;}
	else if(d.DP3 == dpData){
		qDebug() << "dp3 selected";
		MODBUSDATA[5] = 0x03;}
	else
		qDebug() << "DP DATA NOT FOUND";
	return MODBUSDATA;
}

void MainWindow::calculateLCAP()
{
	MODBUSDATA[3] = 0x03;
	int newValue = ui->modbusLCAP->text().toInt();
	if(!newValue)
		return;
	int YKH = newValue/65536;
	int YKL = newValue%65536;

	int YKH_MSB=YKH/256;
	int YKH_LSB=YKH%256;
	int YKL_MSB=YKL/256;
	int YKL_LSB=YKL%256;


	MODBUSDATA[3] = 0x09;
	QString value1 = QString().number(YKH_MSB, 16).prepend("0x");
	QString value2 = QString().number(YKH_LSB, 16).prepend("0x");
	MODBUSDATA[4] = (uchar)value1.toInt(0, 0);
	MODBUSDATA[5] = (uchar)value2.toInt(0, 0);
	autoCRC(MODBUSDATA, 5);
	MODBUSDATA[3] = 0x0A;
	QString value3 = QString().number(YKL_MSB, 16).prepend("0x");
	QString value4 = QString().number(YKL_LSB, 16).prepend("0x");
	qDebug() <<"THIS LINE"<< value1 << value2 <<value3 << value4;
	qDebug() <<"THIS LINE"<< (uchar)value1.toInt(0,16) << value2.toInt(0,16) <<value3.toInt(0,16) << (uchar)value4.toInt(0,16);
	MODBUSDATA[4] = (uchar)value3.toInt(0, 0);
	MODBUSDATA[5] = (uchar)value4.toInt(0, 0);
	autoCRC(MODBUSDATA, 5);
}

void MainWindow::on_modbus_flash_clicked()
{
	if(ui->modbus_selectedbutton->isChecked()){
	MODBUSDATA[2] = 0x00; //2
	MODBUSDATA[3] = 0x00; //3
	MODBUSDATA[4] = 0x00; //4
	ui->modbusstatusbar->setValue(3);
	qDebug() << "FLASHING START";
	modbusSettings(); // 0
	readWrite(); // 1
	timOut(2);
	ui->modbuslabel->setText("STATUS: receiving adress");
	ui->modbus_textedit->appendPlainText("STATUS:  Receiving adress");
	ui->modbusstatusbar->setValue(5);
	timOut(2);
	ui->modbus_textedit->appendPlainText(MODBUSDATA);
	ui->modbuslabel->setText("STATUS: receiving adress");
	ui->modbus_textedit->appendPlainText("STATUS:  Receiving READ/WRITE STATUS ");
	readWrite(); // 1
	ui->modbus_textedit->appendPlainText(MODBUSDATA);
	ui->modbusstatusbar->setValue(16);
	timOut(1);
	ui->modbus_textedit->appendPlainText("STATUS: Detecting null values ");
	ui->modbus_textedit->appendPlainText(MODBUSDATA);
	ui->modbusstatusbar->setValue(26);
	timOut(1);
	ui->modbus_textedit->appendPlainText("STATUS: ASI210 write data will flash after 1 sec");
	ui->modbus_textedit->appendPlainText(MODBUSDATA);
	ui->modbusstatusbar->setValue(31);
	timOut(1);
	paSettings();  //pa
	timOut(2);
	ui->modbus_textedit->appendPlainText("STATUS: Flashing data ...");
	ui->modbus_textedit->appendPlainText(MODBUSDATA);
	ui->modbus_textedit->appendPlainText("STATUS: PA flasging via modbus");
	ui->modbusstatusbar->setValue(43);
	autoCRC(MODBUSDATA, 5);
	timOut(1);
	DPsettings(); // dp modules
	ui->modbus_textedit->appendPlainText("STATUS: Flashing data ........");
	ui->modbus_textedit->appendPlainText(MODBUSDATA);
	ui->modbus_textedit->appendPlainText("STATUS: DP flashing via modbus");
	ui->modbusstatusbar->setValue(69);
	autoCRC(MODBUSDATA,5);
	timOut(1);
	ADsettings();
	ui->modbus_textedit->appendPlainText("STATUS: Flashing data ........");
	ui->modbus_textedit->appendPlainText(MODBUSDATA);
	ui->modbus_textedit->appendPlainText("STATUS:AD flashing via modbus");
	ui->modbusstatusbar->setValue(76);
	autoCRC(MODBUSDATA,5);
	timOut(2);
	calculateLCAP();
	qDebug() << MODBUSDATA << "MODBUS DATA ";
	ui->modbus_textedit->appendPlainText("STATUS:LCAP flashing via modbus");
	ui->modbusstatusbar->setValue(100);
	timOut(1);
	}
	else if(ui->radioButton_default->isChecked()){                         // DEFALT SETTINGS
		modbusSettings();
		MODBUSDATA[1] = 0x06;
		MODBUSDATA[2] = 0x00;
		MODBUSDATA[3] = 0x00;
		MODBUSDATA[4] = 0x00;
		MODBUSDATA[5] = 0x00;

		MODBUSDATA[3] = 0x06; // DP VALUE
		MODBUSDATA[5] = 0x0A; //--------->
		autoCRC(MODBUSDATA, 5);
		MODBUSDATA[3] = 0x08; // AD VALUE
		MODBUSDATA[5] = 0x0;//---------->
		autoCRC(MODBUSDATA, 5);
		MODBUSDATA[3] = 0x07; // PA VALUE
		MODBUSDATA[5] =0x00 ;

		ui->modbusLCAP->setText("DEFAULTVALUE");
		calculateLCAP();
	}
	else
		ui->modbuslabel->setText("PLEASE ");
}

void MainWindow::on_pushButton_3_clicked() // READ ALL
{
	modbusSettings(); //0
	MODBUSDATA[1] = 0x03;
	MODBUSDATA[2] = 0x00;
	MODBUSDATA[3] = 0x01;
	MODBUSDATA[4] = 0x00;
	MODBUSDATA[5] = 0x13;
	autoCRC(MODBUSDATA, 5);
}

void MainWindow::on_pushButton_4_clicked()
{
	if(ui->screenmode->isChecked()){
		serialc->destroyed();
		serialc->closeConnection();
		serialc->openConnetions();
		serialc->screenmode();
	}
	else if(ui->writereadmode->isChecked()){
		serialc->destroyed();
		serialc->closeConnection();
		serialc->openConnetions();
		serialc->readWriteMode();
	}
}

void MainWindow::modbusconsole(QByteArray str){

	if(str.isNull() || str.isEmpty())
		return;
	ui->modbus_textedit->appendPlainText( QString("value: %1").arg(QString::fromUtf8(str)));
}

void MainWindow::on_modbusreadbutton_clicked()
{

	QString str = ui->modbus_readdata->currentText().toLatin1();
	modbusSettings(); //0
	readWrite(); // 1
	MODBUSDATA[2] = 0x00;
	MODBUSDATA[3] = 0x00;
	MODBUSDATA[4] = 0x00;
	MODBUSDATA[5] = 0x00;

	if(str == "POINTER REG"){
		qDebug() << "read pointer";
		MODBUSDATA[3] = 0x03;
		MODBUSDATA[5] = 0x01;
	}else if( str == "RAW SIGNAL(H)" ){
		qDebug() << "rawsignalh";
		MODBUSDATA[3] = 0x01;
		MODBUSDATA[5] = 0x01;
	}else if( str == "RAW SIGNAL(L)" ){
		MODBUSDATA[3] = 0x02;
		MODBUSDATA[5] = 0x01;
	}else if( str == "POINTER VALUE (H)" ){
		MODBUSDATA[3] = 0x04;
		MODBUSDATA[5] = 0x01;
	}else if( str == "POINTER VALUE (L)" ){
		MODBUSDATA[3] = 0x05;
		MODBUSDATA[5] = 0x01;
	}else if( str == "DECIMAL POINT(DP)" ){
		MODBUSDATA[3] = 0x06;
		MODBUSDATA[5] = 0x01;
	}else if( str == "GAIN REG (PA)" ){
		MODBUSDATA[3] = 0x07;
		MODBUSDATA[5] = 0x01;
	}else if( str == "STEP REG (AD)" ){
		MODBUSDATA[3] = 0x08;
		MODBUSDATA[5] = 0x01;
	}else if( str == "LCAP (H)" ){ // R
		MODBUSDATA[3] = 0x09;
		MODBUSDATA[5] = 0x02;
	}else if( str == "LCAP (L)" ){
		MODBUSDATA[3] = 0x0A;
		MODBUSDATA[5] = 0x01;
	}else if( str == "CAL P (H)" ){
		MODBUSDATA[3] = 0x0B;
		MODBUSDATA[5] = 0x01;
	}else if( str == "CAL P (L)" ){
		MODBUSDATA[3] = 0x09;
		MODBUSDATA[5] = 0x01;
	}else if( str == "TARA (H)" ){
		MODBUSDATA[3] = 0x0F;
		MODBUSDATA[5] = 0x01;
	}else if( str == "TARA (L)" ){
		MODBUSDATA[3] = 0x10;
		MODBUSDATA[5] = 0x01;
	}else if( str == "BAUDR (H)" ){
		MODBUSDATA[3] = 0x11;
		MODBUSDATA[5] = 0x01;
	}else if( str == "BAUDR (L)" ){
		MODBUSDATA[3] = 0x12;
		MODBUSDATA[5] = 0x01;
	}else if( str == "BAUDADR" ){
		MODBUSDATA[3] = 0x13;
		MODBUSDATA[5] = 0x01;
		qDebug() << "baudadre";
	}
autoCRC(MODBUSDATA, 5);
}

