#include "Server.hpp"

Server::Server(char *av)
{
    size_t i;
    for(i = 0; isdigit(av[i]) != 0; i++)
    {}
    if (i == strlen(av))
    {
        int port = atoi(av);
        /*etablit une interface de communication entre le serveur et un client
        (renvoie un descripteur)
        AF_INET = domaine d'adresses, ici: IPv4
        SOCK_STREAM = type de socket
        0 = protocole a utiliser, 0 pcq on s en ballec, le systeme gere
        */
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        //checker que socket renvoie pas - 1;
        /*
        _serverAddress contient les informations du serveur
        sin.family specifie le type d'adresse
        inet_addr specifie l'adresse  IP du serveur, ici localhost
        sin.port specifie le port en format reseau
        */
        _serverAddress.sin_family = AF_INET;
        _serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        _serverAddress.sin_port = htons(port);
        /*
        bind lie la socket à l’addresse IP de mon ordi
        */
        bind(_socket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress));
        /*
        listen ecoute via la socket pour détecter des demandes de connexion
        5 est un entier qui represente le nb de connexions autorisees dans la file d attente (nb a changer)
        */
        listen(_socket, 5);
        //checker que ca renvoit pas -1
    }
    else
        std::cout << "invalid port format" << std::endl;
}

int Server::getSocket() const
{
    return _socket;
}

struct sockaddr_in Server::getServerAddress() const
{
    return _serverAddress;
}

struct sockaddr Server::getClientAddr() const
{
    return _clientAddr;
}

int Server::getClientSocket() const
{
    return _client_socket;
}

void Server::setClientSocket(int tmp)
{
    _client_socket = tmp;
    clientSockets_.push_back(_client_socket);
}

void    Server::defineCmd(std::string cmd, int start, int it)
{
    std::string locate;

    locate.append(cmd, start, it - start);
    if (locate.find("NICK") == 0)
    {
        std::cout << "!!!NICK COMMAND!!!" << std::endl;
        nickCmd (locate);
    }
    else if (locate.find("USER") == 0)
        std::cout << "!!!USER COMMAND!!!" << std::endl;
    else if (locate.find("MODE") == 0)
        std::cout << "!!!MODE COMMAND!!!" << std::endl;
    else if (locate.find("PASS") == 0)
        std::cout << "!!!PASS COMMAND!!!" << std::endl;
    else if (locate.find("JOIN") == 0)
        std::cout << "!!!JOIN COMMAND!!!" << std::endl;
    else if (locate.find("PRIVMSG") == 0)
        std::cout << "!!!PRIVMSG COMMAND!!!" << std::endl;
    else if (locate.find("MSG") == 0)
        std::cout << "!!!MSG COMMAND!!!" << std::endl;
    else if (locate.find("KICK") == 0)
        std::cout << "!!!KICK COMMAND!!!" << std::endl;
    else if (locate.find("PING") == 0)
        std::cout << "!!!PING COMMAND!!!" << std::endl;
    else if (locate.find("WHOIS") == 0)
        std::cout << "!!!WHOIS COMMAND!!!" << std::endl;
    else
        std::cout << "???ERROR UNKNOW COMMAND???" << std::endl;
}

void    Server::parser(char *buffer)
{
    std::string cmd = buffer;
    int start = 0;
    for (int i = 0; cmd[i]; i++)
    {
        if (cmd[i] == '\r' && cmd[i + 1] == '\n')
        {
            defineCmd(cmd, start, i);
            i += 2;
            start = i;
        }
    }
}

void Server::nickCmd(std::string str)
{
    std::string cmd = str.substr(str.find(' ') + 1);
    std::cout << "cmd = '" << cmd << "'" << std::endl;
    if (cmd.length() > 9)
        printf("trop de char pour nick");
    for (int i = 0; cmd[i]; i++)
    {
        if(!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
        {
            printf("nickname wrong input");
        }
    }
    _client.setNickName(cmd);
    std::cout << "client _name = '" << _client.getNickName() << "'" << std::endl;  
}

Server::~Server()
{
    close(_socket);
}