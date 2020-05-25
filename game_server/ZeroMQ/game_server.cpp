#include <iostream>
#include <stdlib.h>
#include <zmq.h>
#include <Windows.h>
#include <vector>
#include "Game.h"

int main(int argc, char* argv[])
{
	cGameManger c(40, 20);
	c.Run();
	return 0;	
	//myHandler.myController = NESController;
	while (1)
	{
		//myHandler.send();
		Sleep(100);
	}
}