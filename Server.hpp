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

class Server
{
    private:
        int _socket;
        struct sockaddr_in _serverAddress;
        struct sockaddr _clientAddr;
        int _client_socket;
        std::vector<int> clientSockets_;
    public:
        Server(char *port);
        int getSocket() const;
        struct sockaddr_in getServerAddress() const;
        struct sockaddr getClientAddr() const;
        int getClientSocket() const;
        void setClientSocket(int tmp);
        ~Server();
};