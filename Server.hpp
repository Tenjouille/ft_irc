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
#include "Client.hpp"

class Server
{
    private:
        int _socket;
        struct sockaddr_in _serverAddress;
        struct sockaddr _clientAddr;
        int _client_socket;
        std::vector<int> clientSockets_;
//        std::vector<Client> _clientlst;
        Client _client;
    public:
        Server(char *port);
        int getSocket() const;
        struct sockaddr_in getServerAddress() const;
        struct sockaddr getClientAddr() const;
        int getClientSocket() const;
        void setClientSocket(int tmp);
        void parser(char *buffer);
        void defineCmd(std::string cmd, int start, int it);
        void nickCmd(std::string str);
        ~Server();
};