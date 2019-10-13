# GuessTheNumberQT
## Guess the number game based on the Qt5 library and C++.

Game where you need to find a number between bounds.

The game is made to work over the net : a server waits for clients to connect and launch the game for each client with a different number to find.

Multiple options are available for the client and server. An history of games is available for logged clients.

# How to install

You need cmake with a version >= 3.14 and the last version of QT5 installed on your computer.

got to the root of the repository and type the following :

``mkdir build && cd build``

``cmake ..``

``make``

Binaries will be generated in the server/ and client/ folders.

# How to use

## Serveur

- Server startup options :
    + -h --help (not mandatory): Display the server's help message
    + -v --version (not mandatory): Display the server's version
    + -d --debug (not mandatory): Display debug output [default: off]
    + -b --bounds (not mandatory): X,Y interval to choose a random number from [default: 1,100]
    + -l --limit  (not mandatory): tries limit for a game [default: infinite tries]

## Client

- Client startup options :
    + -h --help (not mandatory): Display the client's help message
    + -v --version (not mandatory): Display the client's version
    + -d --debug (not mandatory): Display debug output [default: off]
    + -h --host (not mandatory): Server's IP [default: 127.0.0.1]
    + -p --port (not mandatory): Server's port [default: 4242]
    + -a --auto (not mandatory): Find the number automaticly [default: off]
    + -n --name (not mandatory): Client's identity (player name)

# dependencies

> CMake >= 3.14

> QT5 13.1

> GTest for unit testing