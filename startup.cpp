#include "startup.h"
#include "ui_startup.h"
#include <QDebug>
#include "mainwindow.h"
#define qcout qDebug()


startup::startup(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::startup)
{
	ui->setupUi(this);
	ui->currenttime->setText(QDate::currentDate().toString());
	connect(ui->password, SIGNAL(textChanged(QString)),this,SLOT(login(QString)));

	QPixmap logo(":/bilkonlogo.png");
	ui->image->setPixmap(logo);
	ui->image->setMinimumSize(160,160);
	ui->image->resize(500,320);
	ui->image->setAlignment(Qt::AlignCenter);
	statlogin = false;
}

startup::~startup()
{
	delete ui;
}

QStringList startup::getInfo()
{
	 pass = ui->password->text();
}

bool startup::login(QString pass)
{
	if(ui->comboBox->currentText().toStdString() == "BILKON-URETIM" && "bilkon" == pass || "BILKON" == pass){
		qcout << "login succesfull";
		ui->toolButton->setStyleSheet(greens);
		ui->loginbutton->setEnabled(false);
		ui->loginbutton->setText("initializing");
		statlogin = true;
		return true;
	}
	else if (ui->comboBox->currentText().toStdString() == "SUPERUSER"
			 && "133224df" == pass || "133224DF" == pass ){
		qcout << "SUPERUSER login succesfull";
		ui->toolButton->setStyleSheet(greens);
		ui->loginbutton->setEnabled(false);
		ui->loginbutton->setText("initializing");
		statlogin = true;
		return true;
	}
	else
	{
	ui->toolButton->setStyleSheet(reds);
	ui->loginbutton->setEnabled(true);
	statlogin = false;
	qcout << "login not succes";
	return false;
	}
}

bool startup::loginstatus()
{
	return statlogin;
}

void startup::delay()
{
	QTime dieTime= QTime::currentTime().addSecs(5);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

