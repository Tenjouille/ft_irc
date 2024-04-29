#ifndef CLIENT_HPP
#define CLIENT_HPP

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
#include "Communication.hpp"
#include "Channel.hpp"

class Channel;

class Client
{
    private:
        struct sockaddr _clientAddr;
        std::string     _nickname;
        std::string     _username;
        std::string     _name;
        int             _socket;
        int             _nickcount;
        int             _connection_status;
        std::string     _tempBuffer;
        //listes de channels dans lequel est le client
        std::vector<std::string> _channel;
        std::vector<std::string> _invitation;
        bool            _dont_set_user;
        bool            _is_connected;
    public:
        Client();
        Client(struct sockaddr client_addr);
        std::string     getNickName() const;
        std::string     getUserName() const;
        std::string     getName() const;
        struct sockaddr getClientAddr() const;
        int             getSocket() const;
        void            setNickName(std::string str);
        void            updateStatus();
        int             getStatus();
        void            setUserName(std::string str);
        void            setSocket(int tmp);
        std::vector<std::string> getChannel();
        std::string     getTempBuffer();
        void            setTempBuffer(std::string str, int flag);
        void	        delChannel(std::string channelName);
        // Client* cloneClient();
        ~Client();

        //to know if we set the user or not depending on value
        void    dont_set_user(bool value);
        bool    do_we_set_or_not() const;

        void    addChannel(std::string to_add);
        void	printChannels();

        //to change connected status
        void	change_connected();
        bool	getConnectedStatus() const;

};

#endif