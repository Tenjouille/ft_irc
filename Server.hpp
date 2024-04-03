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

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define MAGENTA "\033[1;35m"
#define RED "\033[1;31m"
#define UNDER "\033[4m"

class Server
{
    private:
        int _socket;
        struct sockaddr_in      _serverAddress;
        int                     _clientSocket;
        std::map<int, Client*>  _clients;
        std::string             _server_password;
//        std::vector<Client> _clientlst;
        Client _client;
    public:
        Server(char **arguments);
        // struct sockaddr     getClientAddr() const;
        int                 getClientSocket() const;
        std::string         getServerPassword() const;
        void                setClientSocket(int tmp);
        void                setServerPassword(std::string to_set);
        void                parser(char *buffer);
        void                defineCmd(std::string cmd, int start, int it);
        void                nickCmd(std::string str);
        int                 getSocket() const;
        struct sockaddr_in  getServerAddress() const;
        void                newClient(int socket, struct sockaddr client_addr);
        void                loop();
        ~Server();

        void    passCmd(std::string cmd);
        ssize_t sendToClient(std::string to_send);

};
