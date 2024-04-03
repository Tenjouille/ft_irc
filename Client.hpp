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

class Client
{
    private:
        struct sockaddr _clientAddr;
        std::string     _nickname;
        std::string     _username;
        std::string     _name;
        int             _socket;
        int             _nickcount;

    public:
        Client();
        Client(struct sockaddr client_addr);
        std::string     getNickName() const;
        std::string     getUserName() const;
        std::string     getName() const;
        struct sockaddr getClientAddr() const;
        int             getSocket() const;
        void            setNickName(std::string str);
        // void            setUserName();
        // void            setName();
        void            setSocket(int tmp);
        // void            nickCmd(std::string str);
        // void            userCmd(std::string str);
        // void            joinCmd(std::string str);
        ~Client();
};
