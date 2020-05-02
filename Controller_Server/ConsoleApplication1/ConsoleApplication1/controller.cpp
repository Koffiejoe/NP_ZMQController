#include "controller.h"

controller::controller(std::string commPort)
{
	controllerSerial.setPort(commPort);
	controllerSerial.setBaudrate(9600);
	controllerSerial.open();
}

controller::~controller()
{
	controllerSerial.close();
}

std::string controller::getPort()
{
	return controllerSerial.getPort();
}