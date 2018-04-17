#include "application.h"

application::application()
{


}

void application::buttonSettings()
{
	MenuButtons button;
	button.leftShift = "eee";
	qcout << button.leftShift;
	test(&button);
}

bool application::controller(QString text){

	bool controlstatus;
	return controlstatus;
}

void application::test(MenuButtons *menu){
	qcout << "this" << menu->leftShift;
}
