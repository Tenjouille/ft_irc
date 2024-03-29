#include "Server.hpp"

class Command
{
    private:
        std::string _name;
        std::vector<std::string> _args;
        std::string _options;
    public:
        Command(char *buffer);
        std::string getName() const;
        std::vector<std::string> getArgs() const;
        std::string getOptions () const;
        //trouver et executer la bonne commande
        //cmd blabla * nb commande   
        ~Command();
};