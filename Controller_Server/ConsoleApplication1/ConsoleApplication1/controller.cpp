#include "controller.h"
controller::controller()
{
	//send initial handshake to get controller type
	controllerType = 0; //set correct
}

controller::~controller()
{
	//send command to shutdown (so turnoff all enabled things like rumble etc etc)
}

