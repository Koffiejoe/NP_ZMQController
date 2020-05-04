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
	int ID = 0;
	if (argc != 2)
	{
		//std::cout << "Invalid Arguments! Start with ./Controller_Server <ID_OF_SERVER>\n";
	}
	else
	{
		sscanf_s(argv[1], "%d", &ID);
		std::cout << "server started with ID: " << ID;
	}
	
	NES *NESController = new NES("COM5");

	ZMQHandler myHandler("tcp://benternet.pxl-ea-ict.be:24042", "tcp://benternet.pxl-ea-ict.be:24041");
	myHandler.myController = NESController;

	//temp
	int prevVal = 0;
	auto end = std::chrono::steady_clock::now();
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