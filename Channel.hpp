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
        std::string _topic;
        int _topicStatus;
        //liste des clients dans le channel + leurs socket
        std::map<int, Client*>  _clientslst;
    public:
        Channel(std::string str);
        void setName(std::string str);
        void addClient(int socket, Client *client);
        // void  delClient(int socket);
        void setTopic(std::string str);
        int getTopicStatus();
        std::string getTopic();

        std::map<int, Client*> getClientlst();
        std::string getName();
};