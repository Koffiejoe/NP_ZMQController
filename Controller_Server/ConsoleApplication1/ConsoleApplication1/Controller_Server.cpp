#include <iostream>
#include <stdlib.h>
#include <zmq.h>

void* context; //Global context, because you only need one !

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
	//testbdjgohgjklqhngiqds

}