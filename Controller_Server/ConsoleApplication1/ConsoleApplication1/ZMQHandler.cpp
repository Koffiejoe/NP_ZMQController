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
	zmq_ctx_shutdown(context);
	zmq_ctx_term(context); 
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
	int written = 0;	//number of chars written
	if (controllerList.size() == 0)
	{
		return -1;
	}

	for (int contrNum = 0; contrNum < controllerList.size(); ++contrNum)
	{
		//only update when necessary
		if (controllerList[contrNum]->lastUpdate + std::chrono::milliseconds(controllerList[contrNum]->updateSpeed)
			<= std::chrono::steady_clock::now())
		{
			if (controllerList[contrNum]->onlyNewUpdate == true)
			{
				if (controllerList[contrNum]->getRawData() != controllerList[contrNum]->prevRespons) //only update when prevrespons is different
				{
					written = sprintf_s(temp, sizeof(temp), "controllerService!>%d>%s>\0", contrNum, controllerList[contrNum]->getRawData().c_str());

					zmq_send(pushPtr, temp, written + 1, 0); //written + 1 gives term /0
					controllerList[contrNum]->prevRespons = controllerList[contrNum]->getRawData(); //update prev val
				}
			}
			else
			{
				written = sprintf_s(temp, sizeof(temp), "controllerService!>%d>%s>\0", contrNum, controllerList[contrNum]->getRawData().c_str());

				zmq_send(pushPtr, temp, written + 1, 0); //written + 1 gives term /0
				controllerList[contrNum]->prevRespons = controllerList[contrNum]->getRawData(); //update prev val
			}


			controllerList[contrNum]->lastUpdate = std::chrono::steady_clock::now(); //set last updatetime to now
			std::cout << temp << std::endl;
		}

	}

}

int ZMQHandler::sendRespons(std::string* commands)
{
	int cNumber = 0;		//selected controller
	int updateSpeed = 0;
	int updateOnChange = 0;

	//###### set/get something with the controller #######
	if (commands[0] == "contr")
	{
		//decode commands[1] (number of controller)
		try
		{
			cNumber = stoi(commands[1], NULL, 10);
			if (cNumber < 0 || cNumber >(controllerList.size() - 1))	//check if number is within bounds
			{
				zmq_send(pushPtr, ERR_INV_CONTR_NUM, sizeof(ERR_INV_CONTR_NUM), 0);
			}
		}
		catch (std::invalid_argument)					//if NaN: send error and exit
		{
			zmq_send(pushPtr, ERR_INV_CONTR_NUM, sizeof(ERR_INV_CONTR_NUM), 0);
			return(-1);
		}
		//--set updatespeed--
		if (commands[2] == "sUpdate")	//set update speed
		{
			try
			{
				updateSpeed = stoi(commands[3]);
				if (updateSpeed < 10 || updateSpeed > 20000)
				{
					zmq_send(pushPtr, ERR_INV_UPDATE_SPEED, sizeof(ERR_INV_UPDATE_SPEED), 0);
				}
				else
				{
					controllerList[cNumber]->updateSpeed = stoi(commands[3], NULL, 10);
				}
			}
			catch (std::invalid_argument)
			{
				zmq_send(pushPtr, ERR_INV_UPDATE_SPEED, sizeof(ERR_INV_UPDATE_SPEED), 0);
				return(-1);
			}
		}
		//--get updatespeed--
		else if (commands[2] == "gUpdate")
		{
			char temp[50];
			int characters;
			characters = sprintf_s(temp, sizeof(temp), "controllerService!>contr>gUpdate>%d>%d>\0", cNumber, controllerList[cNumber]->updateSpeed); //characters: total ammount
			zmq_send(pushPtr, temp, characters + 1, 0);													//characters + 1 bc it does not add the \0
		}
		//--set rumble--
		else if (commands[2] == "sRumble")
		{
			if (commands[3] == "1")
			{
				controllerList[cNumber]->rumble = true;
				controllerList[cNumber]->sendData();
			}
			else if (commands[3] == "0")
			{
				controllerList[cNumber]->rumble = false;
				controllerList[cNumber]->sendData();
			}
			else
			{
				zmq_send(pushPtr, ERR_INV_3RD_COMM, sizeof(ERR_INV_3RD_COMM), 0);
			}
		}
		//--get rumble--
		else if (commands[2] == "gRumble")
		{
			char temp[50];
			int characters;
			characters = sprintf_s(temp, sizeof(temp), "controllerService!>contr>gRumble>%d>%d>\0", cNumber, controllerList[cNumber]->rumble); //characters: total ammount
			zmq_send(pushPtr, temp, characters, 0);
		}
		//only update when necessary
		else if (commands[2] == "sDiff")
		{
			updateOnChange = stoi(commands[3]);
			if (updateOnChange != 0 && updateOnChange != 1) //has to be 0 or 1
			{
				zmq_send(pushPtr, ERR_INV_UPDATE_SPEED, sizeof(ERR_INV_UPDATE_SPEED), 0);
			}
			else //ok, set it
			{
				controllerList[cNumber]->onlyNewUpdate = stoi(commands[3], NULL, 10);
			}
		}
		//add extra 3rd command here
		else
		{
			zmq_send(pushPtr,ERR_INV_3RD_COMM, sizeof(ERR_INV_3RD_COMM), 0);
			return(-1);
		}

	}

	//server commands
	else if (commands[0] == "serv")
	{
		if (commands[1] == "gList")
		{
			std::string list;
			list.append("controllerService!>serv>gList>");
			for (int i = 0; i < controllerList.size(); ++i)
			{
				list.append(controllerList[i]->type);
				if (i < (controllerList.size() - 1))		//don't add last ,
				{
					list.append(",");
				}
			}
			list.append(">\0");
			zmq_send(pushPtr, list.c_str(), list.size(), 0);
		}
	}
	else
	{
		zmq_send(pushPtr, ERR_INV_1ST_COMM, sizeof(ERR_INV_1ST_COMM), 0);
		return(-1);
	}
	return(0);
}