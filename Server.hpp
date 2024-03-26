#include <sys/types.h> 
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

class Server
{
    private:
        const std::string _name;
        int _socket;
        struct sockaddr_in _serverAddress;
    public:
        Server(char *port);
        // const std::string getName() const;
        // const int getSocket() const;
        ~Server();
};