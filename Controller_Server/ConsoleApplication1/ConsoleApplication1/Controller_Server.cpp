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
#include "ZMQHandler.h"

int main(int argc, char* argv[])
{

	std::cout << "########----ControllerServer Started! ----########" << std::endl;
	std::cout << "##################################################" << std::endl << std::endl;
	NES *NESController = new NES("COM5");

	ZMQHandler myHandler("tcp://benternet.pxl-ea-ict.be:24042", "tcp://benternet.pxl-ea-ict.be:24041");
	myHandler.controllerList.push_back(NESController);
	myHandler.controllerList.push_back(NESController);//test

	//temp
	int prevVal = 0;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//---
	while (1)
	{
		if (end <= std::chrono::steady_clock::now())
		{
			myHandler.send();
			end = std::chrono::steady_clock::now() + std::chrono::milliseconds(myHandler.updateSpeed);
		}
		
		myHandler.recv();

		if (myHandler.updateSpeed != prevVal)
		{
			prevVal = myHandler.updateSpeed;
		}
		Sleep(100);
	}
		
	
	
	delete NESController;
}