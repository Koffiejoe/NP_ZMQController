#include <iostream>
#include <stdlib.h>
#include <zmq.h>
#include <Windows.h>

#include "controller.h"
#include "NES.h"

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
	
	//NES *myController = new NES("COM5");

	char buffer[500] = "controllerService?>aapje>";					//entire incoming string
	char* commandPtr, command[15];		//command part
	char* argumentsPtr, arguments[100];	//arguments part
	
	commandPtr = strchr(buffer, '>');
	argumentsPtr = strchr(commandPtr + 1, '>');
	std::cout << commandPtr << std::endl;
	std::cout << sizeof(buffer) << std::endl;
	//delete myController;
}