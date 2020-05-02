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
		std::cout << "Invalid Arguments! Start with ./Controller_Server <ID_OF_SERVER>\n";
	}
	else
	{
		sscanf_s(argv[1], "%d", &ID);
		std::cout << "server started with ID: " << ID;
	}
	
	NES *myController = new NES("COM5");



	delete myController;
}