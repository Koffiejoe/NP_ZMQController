# Network Programming Pong game 
Made by students of [the hogeschool PXL](https://www.pxl.be).

The purpose of this project is to develop a simple C/C++ application through a client/server.

For this Project the [ZMQ Library](https://zeromq.org/languages/cplusplus/) is used.

*On the client you control the rackets for the pong game. Data gets send through the broker to the game where the rackets respond to the clients commands.
*The rackets are controlled by using 'z' and 's' for the left racket and 'i' and 'k' for the right racket.
*The broker receives the following message: 
	*user>command number>command. i.e. bart>1>up1.
*You can quit the game by using the 'q' button on your keyboard.

Playfield:
<p align="center"><img src="playfield.PNG"></p>

score:
<p align="center"><img src="score.PNG"></p>

Data on the broker:

<p align="center"><img src="broker.png"></p>

## Troubleshooting
*during this project there are issues with filtering the buffer correctly. 
*The buffer gets filled with 'junk' data after the message has been inserted. This has been fixed with resizing the buffer.

<p align="center"><img src="buffer.png"></p>

* Bart Gripsen      [LinkedIn](linkedin.com/in/bart-grispen-9634b1181)