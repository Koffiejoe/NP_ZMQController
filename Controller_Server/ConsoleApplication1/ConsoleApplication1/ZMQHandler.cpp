#include "ZMQHandler.h"
ZMQHandler::ZMQHandler(const char* sub, const char* push)
{
	char filterString[] = "controllerService?>";
	context = zmq_ctx_new();
	pushPtr = zmq_socket(context, ZMQ_PUSH);
	subPtr = zmq_socket(context, ZMQ_SUB);

	zmq_connect(pushPtr, push);
	zmq_connect(subPtr, sub);

	//subscribe to channel of server 
	zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, filterString, sizeof(filterString) - 1);
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

	if (myController == NULL)
	{
		return -1;
	}

	if (zmq_recv(subPtr, buffer, sizeof(buffer), ZMQ_NOBLOCK) == -1)
	{
		return -1;
	}
	//Decode the message received
	std::string commands[4];			//the commands
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
	int cNumber = 0;					//move when everything is in vecotr!!!
	//--set something with the controller--
	if (commands[0] == "sControl")
	{
		//get number of controller and check if correct
		try
		{
			cNumber = stoi(commands[1], NULL, 10);
		}
		//if not correct: send error and exit
		catch(std::invalid_argument)
		{
			zmq_send(pushPtr, "controllerService!>err>INV_CONTR_NUM>", 38, 0);	
			return(-1);
		}
		

		if (commands[2] == "sUpdate")
		{
			try
			{
				updateSpeed = stoi(commands[3], NULL, 10);
			} //no number
			catch(std::invalid_argument)
			{
				zmq_send(pushPtr, "controllerService!>err>INV_UPD_SPEED>", 38, 0);
				return(-1);
			}
		}
		//add extra 3rd commands here
		else
		{
			zmq_send(pushPtr, "controllerService!>err>INV_3RD_COM>", 36, 0);
			return(-1);
		}

	}
	//--get updatespeed
	else if (commands[0] == "gUpdate")
	{
		//get number of controller and check if correct
		try
		{
			cNumber = stoi(commands[1], NULL, 10);
		}
		//if not correct: send error and exit
		catch (std::invalid_argument)
		{
			zmq_send(pushPtr, "controllerService!>err>INV_CONTR_NUM>", 38, 0);
			return(-1);
		}
		//add controller number check, but is not yet here
		char temp[50];
		int characters;
		characters = sprintf_s(temp, sizeof(temp), "controllerService!>gUpdate>%d>%d>\0",cNumber, updateSpeed); //characters: total ammount
		zmq_send(pushPtr, temp, characters + 1, 0);													//characters + 1 bc it does not add the \0
	}

	else
	{
		zmq_send(pushPtr, "controllerService!>err>INV_1ST_COM>", 36, 0);
		return(-1);
	}
	return(1);
}

int ZMQHandler::send()
{
	std::string command = "controllerService!>1>";

	if (myController == NULL)
	{
		return -1;
	}

	command += myController->getRawData();
	command += '>';
	const void* cvCommand = command.c_str();
	zmq_send(pushPtr, cvCommand, sizeof(command), 0);
}