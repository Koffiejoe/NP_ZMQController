#include "controller.h"

controller::controller(std::string commPort)
{
	controllerSerial.setPort(commPort);
	controllerSerial.setBaudrate(9600);
	lastUpdate = std::chrono::steady_clock::now();
	try
	{
		controllerSerial.open();
	}
	catch (serial::IOException) //i.e. if the commport is not in use
	{
		std::cout << "Error opening port " << commPort << std::endl;
	}
}

controller::~controller()
{
	if (controllerSerial.isOpen())
	{
		controllerSerial.close();
	}
}

std::string controller::getPort()
{
	return controllerSerial.getPort();
}