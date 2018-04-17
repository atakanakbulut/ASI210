#ifndef STARTUP_H
#define STARTUP_H

#include <QDialog>
#include <QTime>

#define reds "background-color: red"
#define blues "background-color: blue"
#define grays "background-color: gray"
#define greens "background-color: green"

namespace Ui {
class startup;
}

class startup : public QDialog
{
	Q_OBJECT

public:
	explicit startup(QWidget *parent = 0);
	~startup();
	QString usern;
	QString pass;

	bool statlogin;
public slots:
	bool login(QString str);

	bool loginstatus();

	void delay();
private:
	Ui::startup *ui;
	QStringList getInfo();

private slots:

};

#endif // STARTUP_H
