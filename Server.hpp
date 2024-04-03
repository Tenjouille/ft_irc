#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h> 
# include <sys/socket.h>
# include <iostream>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <vector>
# include <map>
# include "Client.hpp"

class Client;

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
        ~Server();
        int                 getSocket() const;
        struct sockaddr_in  getServerAddress() const;
        int                 getClientSocket() const;
        void                setClientSocket(int tmp);
        void                parser(char *buffer);
        void                defineCmd(std::string cmd, int start, int it);
        void                newClient(int socket, struct sockaddr client_addr);
        void                loop();
};

#endif