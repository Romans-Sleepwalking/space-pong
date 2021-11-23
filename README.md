# Course Project Repository

The project will happen in three stages:

* Basic networking
* Basic prototype
* Final program

After each stage You must complete a questionnaire in Ortus, and I will check the state of Your repository at the due date of that stage.

## Basic networking
During this stage You must create simple server and client code. It is advised to create libraries for commonly used things, so You can include them in both server and client code.

In this stage the server must be created, with the following reqirements:

* it can be ran with port as parameter `server -p=12345`. You can also add more parameters if You want.
* The server must have one main "game loop" process, that currently just waits for input/keypress to end, as well as "network" process, that listens for connections on the specified port. 
* The server must accept all connections by the clients as separate process (no need for special packet parsing for now, just accept and fork the connection)
* All the processes must have access to the same shared memory buffer. For now it can be small (e.g. 1000 bytes) but You will increase it in the future probably.
* In this shared memory an array of integers must be stored. Each integer represents a client process, and stores how many bytes the client has sent to the server.
* The forked connections must read data from their client connections byte by byte. They must keep a number of bytes recieved (N) as a variable and store the updated value in the mentioned shared memory integer array.
* After receiving each character the client connection process must respond to the client with the same received character, but **N times for that specific client**.

Also the client must be created with the following requirements:

* It takes IP address and PORT as parameters `client -a=123.123.123.123 -p=12345`. You can also add more parameters if You want.
* The client must then connect to the server in a separate thread.
* When ever server sends some data to the client, the client must print that data to terminal
* In the main thread the client should be able to input text in console, and that text must be sent to the server.

As a result You should see something like this (it can also send/print additional enter characters and null characters):

    server -p=12345
    client -a=localhost -p=12345
    > abc
    abbccc
    > XY
    XXXXYYYYY
  
## Basic prototype
In this stage You must implement full networking according to the project API.
  
The server and client must recognize all relevant packets, store their data and call (currently non-functional) functions, that will process the content of these packets.

To test the system for now You can just hard-code what packets will be sent.

After this step the only thing left to program is actual game logic and visuals.

## Final program
It should work as described in the API document. The packets acquired in the basic prototype stage must be explored, and used to actually visualize the game play (You can use ready libraries for graphics or movin text).

Remember that at any stage some other client/server combo should also be able to work with each other based on the API
