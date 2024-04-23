#pragma once
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::string _key;
        int         _limit;
        bool        _inviteonly;
        int _topicStatus;
        //liste des clients dans le channel + leurs socket
        std::map<int, Client*>  _clientslst;
        std::map<int, Client*>  _operators;
    public:
        Channel(std::string str);
        void    infoChannel();
        void    setName(std::string str);
        void    addClient(int socket, Client *client);
        void    changeOperator(char sign, std::vector<std::string>& args);
        void    newOperator(int socket, Client *client);
        void    changeInvit(char sign);
        void    changeTopic(char sign, std::vector<std::string>& args);
        void    changeKey(char sign, std::vector<std::string>& args);
        void    changeLimit(char sign, std::vector<std::string>& args);
        void	execMode(std::string options, std::vector<std::string>& args);
        void    defineMode(char sign, char option, std::vector<std::string>& args);
        void setTopic(std::string str);
        int getTopicStatus();
        std::string getTopic();
        void    removeClientFromLst(std::string clientName);    
        std::map<int, Client*> getClientlst();
        std::string getName();
};