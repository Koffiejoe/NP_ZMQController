#include <iostream>
#include <time.h>
#include <conio.h>
#include <zmq.h>
#include "ZMQHandler.h"
#include <string.h>
using namespace std;
enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };

class cBall
{
private:
	int x, y;
	int originalX, originalY;
	eDir direction;
public:
	cBall(int posX, int posY)
	{
		originalX = posX;
		originalY = posY;
		x = posX; y = posY;
		direction = STOP;
	}
	void Reset()
	{
		x = originalX; y = originalY;
		direction = STOP;
	}
	void changeDirection(eDir d)
	{
		direction = d;
	}
	void randomDirection()
	{
		direction = (eDir)((rand() % 6) + 1);
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline eDir getDirection() { return direction; }
	void Move()
	{
		switch (direction)
		{
		case STOP:
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case UPRIGHT:
			x++; y--;
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		default:
			break;
		}
	}
	friend ostream& operator<<(ostream& o, cBall c)
	{
		o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]";
		return o;
	}
};
class cPaddle
{
private:
	int x, y;
	int originalX, originalY;
public:
	cPaddle()
	{
		x = y = 0;
	}
	cPaddle(int posX, int posY) : cPaddle()
	{
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	}
	inline void Reset() { x = originalX; y = originalY; }
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUp() { y--; }
	inline void moveDown() { y++; }
	friend ostream& operator<<(ostream& o, cPaddle c)
	{
		o << "Paddle [" << c.x << "," << c.y << "]";
		return o;
	}
};
class cGameManger
{
private:
	int width, height;
	int score1, score2;
	char up1, down1, up2, down2, stop_quit;
	bool quit;
	cBall* ball;
	cPaddle* player1;
	cPaddle* player2;
	void* pushPtr, * subPtr, * context, * context_2;
public:
	cGameManger(int w, int h)
	{
		srand(time(NULL));
		quit = false;
		up1 = 'z'; up2 = 'i';
		down1 = 's'; down2 = 'k';
		stop_quit = 'q';
		score1 = score2 = 0;
		width = w; height = h;
		ball = new cBall(w / 2, h / 2);
		player1 = new cPaddle(1, h / 2 - 3);
		player2 = new cPaddle(w - 2, h / 2 - 3);
		context = zmq_ctx_new();
		context_2 = zmq_ctx_new();
		pushPtr = zmq_socket(context, ZMQ_PUSH);
		subPtr = zmq_socket(context_2, ZMQ_SUB);


		zmq_connect(pushPtr, "tcp://benternet.pxl-ea-ict.be:24041");
		zmq_connect(subPtr, "tcp://benternet.pxl-ea-ict.be:24042");

		//subscribe to channel of server 
		zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, "bart>", 5);
	}
	~cGameManger()
	{
		delete ball, player1, player2;
	}
	void ScoreUp(cPaddle* player)
	{
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;

		ball->Reset();
		player1->Reset();
		player2->Reset();
	}
	void Draw()
	{
		system("cls");
		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
		cout << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int ballx = ball->getX();
				int bally = ball->getY();
				int player1x = player1->getX();
				int player2x = player2->getX();
				int player1y = player1->getY();
				int player2y = player2->getY();

				if (j == 0)
					cout << "\xB2";

				if (ballx == j && bally == i)
					cout << "O"; //ball
				else if (player1x == j && player1y == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y == i)
					cout << "\xDB"; //player2

				else if (player1x == j && player1y + 1 == i)
					cout << "\xDB"; //player1
				else if (player1x == j && player1y + 2 == i)
					cout << "\xDB"; //player1
				else if (player1x == j && player1y + 3 == i)
					cout << "\xDB"; //player1

				else if (player2x == j && player2y + 1 == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y + 2 == i)
					cout << "\xDB"; //player1
				else if (player2x == j && player2y + 3 == i)
					cout << "\xDB"; //player1
				else
					cout << " ";

				if (j == width - 1)
					cout << "\xB2";
			}
			cout << endl;
		}

		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
		cout << endl;

		
	}
	void Input()
	{
		ball->Move();

		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();
		
		if (_kbhit())
		{
			char current = _getch();
			if (current == up1)
				if (player1y > 0)
					zmq_send(pushPtr, "bart>1>up1>", 11, 0);
			if (current == up2)
				if (player2y > 0)
					zmq_send(pushPtr, "bart>1>up2>", 11, 0);
			if (current == down1)
				if (player1y + 4 < height)
					zmq_send(pushPtr, "bart>1>down1>", 13, 0);
			if (current == down2)
				if (player2y + 4 < height)
					zmq_send(pushPtr, "bart>1>down2>", 13, 0);
			if (ball->getDirection() == STOP)
				ball->randomDirection();

			if (current == stop_quit) {
				zmq_send(pushPtr, "bart>quit>", 10, 0);
			}
		}
		string commands[4];			//the commands
		char buffer[265];
		string temp_buffer;
		int length;
		//temp_buffer.clear();
		length = zmq_recv(subPtr, buffer, sizeof(buffer), ZMQ_NOBLOCK);
		//Decode the message received
		char* currPosPtr = buffer;
		char* nextPosPtr = buffer;
		char temp[20];
		string Player1Name;
		string Player2Name;
		
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

		if (commands[1] == "up1")
			if (player1y > 0) {
				player1->moveUp();
			}
		if (commands[1] == "up2")
			if (player2y > 0) {
				player2->moveUp();
			}
		if (commands[1] == "down1")
			if (player1y + 4 < height) {
				player1->moveDown();
			}
		if (commands[1] == "down2")
			if (player2y + 4 < height) {
				player2->moveDown();
			}
		if (commands[0] == "quit")
			{
				quit = true;
				cout << "quit";
			}
		//add playernames
		if (commands[0] == "1" && commands[1] == "join")
		{
			Player1Name = commands[2];
			//Player1Name = "links";
		}
		if (commands[0] == "2" && commands[1] == "join")
		{
			Player2Name = commands[2];
			//Player2Name = "rechts";
		}
		cout << Player1Name << score1 << endl << Player2Name << score2 << endl; //write playernames to terminal*/
		//cout << "score1: " << score1 << endl << "score2: " << score2 << endl;
	}
	void Logic()
	{
		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player2x = player2->getX();
		int player1y = player1->getY();
		int player2y = player2->getY();

		//left paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player1x + 1)
				if (bally == player1y + i)
					ball->changeDirection((eDir)((rand() % 3) + 4));

		//right paddle
		for (int i = 0; i < 4; i++)
			if (ballx == player2x - 1)
				if (bally == player2y + i)
					ball->changeDirection((eDir)((rand() % 3) + 1));

		//bottom wall
		if (bally == height - 1)
		{
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
			zmq_send(pushPtr, "bart>1>ballup>", 14, 0);
		}
		//top wall
		if (bally == 0) {
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
			zmq_send(pushPtr, "bart>1>balldown>", 16, 0);
		}
		//right wall
		if (ballx == width - 1)
			ScoreUp(player1);
		//left wall
		if (ballx == 0)
			ScoreUp(player2);
		/*const void positionx() == ballx;

		zmq_send(pushPtr, positionx, 11, 0);
		zmq_send(pushPtr, bally, 11, 0);*/
	}
	void Run()
	{
		while (!quit)
		{
			Draw();
			Input();
			Logic();
		}
	}
};