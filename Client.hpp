#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class Client
{
private:
    std::string     _nickname;
    std::string     _username;
    std::string     _name;
    struct sockaddr _clientAddr;
public:
    Client();
    Client(struct sockaddr client_addr);
    std::string     getNickName() const;
    std::string     getUserName() const;
    std::string     getName() const;
    struct sockaddr getClientAddr() const;
    // void        setNickName() const;
    // void        setUserName() const;
    // void        setName() const;
    ~Client();
};

#endif