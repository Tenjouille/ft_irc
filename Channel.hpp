#pragma once
#include "Server.hpp"

class Channel
{
    private:
        std::string _name;
        std::map<int, Client*>  _clientslst;
    public:
        Channel(std::string str);
        void setName(std::string str);
        void addClient(int socket);
        std::string getName();
};