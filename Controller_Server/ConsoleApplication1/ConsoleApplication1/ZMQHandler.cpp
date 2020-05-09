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
	
	char buffer[500];			//holds the entire incoming data

	if (controllerList.size() == 0)
	{
		return -1;
	}

	//only reads when data is available, else just returns
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
	sendRespons(commands);


	return(1);
}

int ZMQHandler::send()
{
	char temp[100];
	if (controllerList.size() == 0)
	{
		return -1;
	}

	for (int contrNum = 0; contrNum < controllerList.size(); ++contrNum)
	{
		;
		sprintf_s(temp, sizeof(temp), "controllerService!>%d>%s>\0", contrNum, controllerList[0]->getRawData().c_str());
		std::cout << "Data: " << controllerList[0]->getRawData() << std::endl;
		std::cout << "Send: " << temp << std::endl;
		zmq_send(pushPtr, temp, sizeof(temp), 0);
	}

}

int ZMQHandler::sendRespons(std::string* commands)
{
	int cNumber = 0;
	//###### set something with the controller #######
	if (commands[0] == "sControl")
	{
		try												//get number of controller and check if a number
		{
			cNumber = stoi(commands[1], NULL, 10);
		}
		catch (std::invalid_argument)					//if NaN: send error and exit
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
			catch (std::invalid_argument)
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


	//######## get updatespeed ########
	else if (commands[0] == "gUpdate")
	{
		try													//get number of controller and check if correct
		{
			cNumber = stoi(commands[1], NULL, 10);
		}
		catch (std::invalid_argument)						//if not correct: send error and exit
		{
			zmq_send(pushPtr, "controllerService!>err>INV_CONTR_NUM>", 38, 0);
			return(-1);
		}
		//add controller number check, but is not yet here
		char temp[50];
		int characters;
		characters = sprintf_s(temp, sizeof(temp), "controllerService!>gUpdate>%d>%d>\0", cNumber, updateSpeed); //characters: total ammount
		zmq_send(pushPtr, temp, characters + 1, 0);													//characters + 1 bc it does not add the \0
	}

	else
	{
		zmq_send(pushPtr, "controllerService!>err>INV_1ST_COM>", 36, 0);
		return(-1);
	}
	return(0);
}