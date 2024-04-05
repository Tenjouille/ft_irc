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
        std::string             _password;
        int _flag;
//        std::vector<Client> _clientlst;
        //Client _client;
    public:
        Server(char **arguments);
        // struct sockaddr     getClientAddr() const;
        int                 getClientSocket(int socket) const;
        void                setClientSocket(int tmp);
        void                parser(char *buffer, int socket);
        void                defineCmd(std::string cmd, int start, int it, int socket);
        void                nickCmd(std::string str, int socket);
        int                 getSocket() const;
        struct sockaddr_in  getServerAddress() const;
        void                loop_bis(fd_set all_sockets, fd_set read_fds, int fd_max);
        void                read_data_from_socket(int socket, fd_set *all_sockets, int fd_max, int server_socket);
        void                accept_new_connection(int server_socket, fd_set *all_sockets, int *fd_max);
        void                loop();
        void                caplsCmd(std::string locate, int socket);

        ~Server();

        std::string getServerPassword() const;
        void    passCmd(std::string cmd, int socket);
        ssize_t sendToClient(std::string to_send, int socket);

};
