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
	connect(sock,SIGNAL(newUdpData(QString)), ui->console, SLOT(appendPlainText(QString)));
	connect(serialc, SIGNAL(speak(QByteArray)),this, SLOT(showLCDLabel2(QByteArray)));
	connect(serialc, SIGNAL(writtenData(QString)),ui->console, SLOT(appendPlainText(QString)));
	connect(tim2, SIGNAL(timeout()), ui->console, SLOT(clear()));



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
	ba[0] = 0x42;
	ba[1] = 0x55;
	ba[2] = 0x54;
	ba[3] = 0x54;
	ba[4] = 0x4f;
	ba[5] = 0x4e;
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
