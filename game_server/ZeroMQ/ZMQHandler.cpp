#include "ZMQHandler.h"
ZMQHandler::ZMQHandler(const char* sub, const char* push)
{
	context = zmq_ctx_new();
	pushPtr = zmq_socket(context, ZMQ_PUSH);
	subPtr = zmq_socket(context, ZMQ_SUB);

	zmq_connect(pushPtr, push);
	zmq_connect(subPtr, sub);

	//subscribe to channel of server 
	zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, "controllerService?>", 20);
	zmq_//SUB op Game server
}

ZMQHandler::~ZMQHandler()
{
	zmq_close(pushPtr);
	zmq_close(subPtr);
	zmq_ctx_shutdown(context); //optional for cleaning lady order (get ready you l*zy f*ck)
	zmq_ctx_term(context); //cleaning lady goes to work
}

int ZMQHandler::recv()
{
	char buffer[500];					//entire incoming string
	int controllernumber;
	char movement[4];
	/*if (myController == NULL)
	{
		return -1;
	}*/
	zmq_recv(subPtr, buffer, sizeof(buffer), 0);
	//Decode the message received
	char* commands[4];			//the commands
	char* currPosPtr = buffer;
	char* nextPosPtr = buffer;
	char temp[20];

	for (short i = 0; i < 4; ++i)  //max commands = 4
	{
		currPosPtr = strchr(currPosPtr + 1, '>');							//find where the > character is: shifts up each time
		if (currPosPtr == NULL) { break; }									//for < 3 commands

		nextPosPtr = strchr(currPosPtr + 1, '>');
		if (nextPosPtr == NULL) { break; }									//to not include the last >
		//should add break if memcpy length > sizeof buffer - 1

		memcpy(temp, currPosPtr + 1, (nextPosPtr - currPosPtr) - 1);		//copy the length of the command (excluding the start and end >)
		temp[(nextPosPtr - currPosPtr) - 1] = NULL;							//add terminating NULL
		commands[i] = temp;
	}

	//decoding and sending respons
	int cNumber = 0;					//this should not be here
	int updateSpeed = 0;                //this should be not here but in .h
	//--set something with the controller--
	if (commands[0] == "sControl") //zogezegd player commando 1 vd2 
	{
		//get number of controller and check if correct
		if (strtol(commands[1], NULL, 10) != 0 && errno != LONG_MAX && errno != LONG_MIN)
		{
			cNumber = strtol(commands[1], NULL, 10);
		}
		//if not correct: send error and exit
		else
		{
			std::string player;
			player.append("Bart>");
			player.append(controllernumber);
			zmq_send(pushPtr, player.c_str(), player.size(), 0); //"controllerService!>1>up>"
			return(-1);
		}


		if (commands[2] == "sUpdate") //zogezegd movement
		{
			if (strtol(commands[3], NULL, 10) != 0 && errno != LONG_MAX && errno != LONG_MIN)
			{
				updateSpeed = strtol(commands[1], NULL, 10);
			}
			else
			{
				std::string player;
				player.append("Bart>");
				player.append(controllernumber);
				player.append(">");
				player.append(movement);
				zmq_send(pushPtr, player.c_str(), player.size(), 0); //"controllerService!>1>up>"
				return(-1);
			}
		}
		//add extra 3rd commands here
		else
		{
			zmq_send(pushPtr, "controllerService!>err>INV_3RD_COM>", 38, 0);
			return(-1);
		}

	}
	//--get list of controllers connected--
	else
	{
		zmq_send(pushPtr, "controllerService!>err>INV_1ST_COM>", 38, 0);
		return(-1);
	}
}

int ZMQHandler::send()
{
	std::string command = "controllerService!>1>";

	/*if (myController == NULL)
	{
		return -1;
	}*/

	//command += myController->getRawData();
	command += '>';
	const void* cvCommand = command.c_str();
	std::cout << "sending: " << zmq_send(pushPtr, cvCommand, sizeof(command), 0) << std::endl;
}