#include "Server.hpp"

class Client
{
    private:
        std::string _nickname;
        std::string _username;
        std::string _name;

    public:
        Client();
        std::string getNickName() const;
        std::string getUserName() const;
        std::string getName() const;
        void setNickName() const;
        void setUserName() const;
        void setName() const;
        ~Client();
};