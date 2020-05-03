#include <iostream>
#include <stdlib.h>
#include <zmq.h>
#include <Windows.h>
#include <vector>

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
	
	//NES *myController = new NES("COM5");

	char buffer[500] = "controllerService?>contr1>sRefrese>2000>";					//entire incoming string
	char commands[3][20];
	char* currPosPtr = buffer;
	char* nextPosPtr = buffer;
	
	for (short i = 0; i < 3; ++i)  //max commands = 3
	{
		currPosPtr = strchr(currPosPtr + 1, '>');	//find where the > character is: shifts up each time
		if (currPosPtr == NULL) { break; }				//for < 3 commands

		nextPosPtr = strchr(currPosPtr + 1, '>');
		if (nextPosPtr == NULL) { break; }			//to not include the last >

		memcpy(commands[i], currPosPtr + 1, (nextPosPtr - currPosPtr) - 1); //copy the length of the command (excluding the start and end >)
		commands[i][(nextPosPtr - currPosPtr)-1] = NULL;					//add terminating NULL
	}
	std::cout << commands[0] << std::endl;
	std::cout << commands[1] << std::endl;
	std::cout << commands[2] << std::endl;
	
	//delete myController;
}