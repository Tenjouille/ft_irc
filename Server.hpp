#pragma once
#include <sys/types.h> 
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include "Client.hpp"

class Server
{
    private:
        int _socket;
        struct sockaddr_in      _serverAddress;
        int                     _clientSocket;
        std::map<int, Client*>  _clients;
//        std::vector<Client> _clientlst;
    public:
        Server(char *port);
        // struct sockaddr     getClientAddr() const;
        int                 getClientSocket() const;
        void                setClientSocket(int tmp);
        void                parser(char *buffer);
        void                defineCmd(std::string cmd, int start, int it);
        void                nickCmd(std::string str);
        int                 getSocket() const;
        struct sockaddr_in  getServerAddress() const;
        void                newClient(int socket, struct sockaddr client_addr);
        void                loop();
        ~Server();
};
