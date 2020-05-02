#include <iostream>
#include <stdlib.h>
#include <zmq.h>
#include <serial.h>
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
	serial::Serial test;
	test.setPort("COM5");
	test.open();
	test.write("init\n");
	std::cout << test.readline();
}