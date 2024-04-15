#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Server;
class Client;

class Channel
{
    private:
        std::string _name;
        std::map<int, Client*>  _clientslst;
    public:
        Channel(std::string str);
        void setName(std::string str);
        void addClient(int socket, Client *client);
        std::map<int, Client*> getClientlst();
        std::string getName();
};