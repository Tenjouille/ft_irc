#include "Client.hpp"

Client::Client()
{
    std::cout << "2New client" << std::endl;
    _nickcount = 0;

}

Client::Client(struct sockaddr client_addr) : _clientAddr(client_addr)
{
    std::cout << "1New client" << std::endl;
}

std::string Client::getNickName() const
{
    return _nickname;
}

std::string Client::getUserName() const
{
    return _username;
}

std::string Client::getName() const
{
    return _name;
}

struct sockaddr Client::getClientAddr() const
{
    return (_clientAddr);
}
// void Client::setNickName() const
void Client::setSocket(int tmp)
{
    _socket = tmp;
}

int Client::getSocket() const
{
    return _socket;
}

// void Client::nickCmd(std::string str)
// {
//     if (str.length() > 9)
//         printf("trop de char pour nick");
//     for (int i = 0; str[i]; i++)
//     {
//         if(!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')))
//         {
//             printf("nickname wrong input");
//         }
//     }
//     _nickname = str;
// }

// void Client::userCmd(std::string str)
// {
//  //   Parameters: <username> <hostname> <servername> :<realname>
// //idk
// }

// void Client::joinCmd(std::string str)
// {

// }

void Client::setNickName(std::string str)
{
    _nickname = str;
}

// void Client::setUserName() const
// {

// }

// void Client::setName() const
// {

// }

Client::~Client()
{}