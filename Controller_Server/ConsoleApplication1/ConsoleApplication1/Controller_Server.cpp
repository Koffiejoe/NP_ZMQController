#include <iostream>
#include <stdlib.h>
#include <zmq.h>
#include <Windows.h>
#include <vector>

//temporarily
#include <chrono>
//-----------

#include "controller.h"
#include "NES.h"
#include "GC.h"
#include "ZMQHandler.h"

int main(int argc, char* argv[])
{

	std::cout << "########----ControllerServer Started! ----########" << std::endl;
	std::cout << "##################################################" << std::endl << std::endl;

	GC* GCController = new GC("COM10");
	NES* NESController = new NES("COM5");
	


	ZMQHandler myHandler("tcp://benternet.pxl-ea-ict.be:24042", "tcp://benternet.pxl-ea-ict.be:24041");
	myHandler.controllerList.push_back(GCController);
	myHandler.controllerList.push_back(NESController);
	myHandler.controllerList.at(0)->updateSpeed = 1000;
	myHandler.controllerList.at(1)->updateSpeed = 500;
	while (1)
	{
		myHandler.send();
	}
	delete GCController;
}