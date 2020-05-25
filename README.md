# Network Programming Pong game 
Made by students of [the hogeschool PXL](https://www.pxl.be).

The purpose of this project is to develop a simple C/C++ application through a server.

for this Project the [ZMQ Library](https://zeromq.org/languages/cplusplus/) is used.

on the client you control the rackets for the pong game. data gets send through the broker to the game where the rackets respond to the clients commands.
the rackets are controlled by using 'z' and 's' for the left racket and 'i' and 'k' for the right racket.
the broker receives the following message: user>command number>command. i.e. bart>1>up1
maximum score can be set with the numbers on your keyboard

Data on the broker:

<p align="center"><img src="broker.png"></p>

## Troubleshooting
during this project there are issues with filtering the buffer correctly.

<p align="center"><img src="buffer.png"></p>

* Bart Gripsen      [LinkedIn](linkedin.com/in/bart-grispen-9634b1181)