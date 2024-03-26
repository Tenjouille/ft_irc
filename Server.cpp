#include "Server.hpp"

Server::Server(char *av)
{
    size_t i;
    for(i = 0; isdigit(av[i]) != 0; i++)
    {}
    if (i == strlen(av))
    {
        int port = atoi(av);
        std::cout << port << std::endl;

        /*etablit une interface de communication entre le serveur et un client
        (renvoie un descripteur)
        AF_INET = domaine d'adresses, ici: IPv4
        SOCK_STREAM = type de socket
        0 = protocole a utiliser, pca on s en ballec, le systeme gere
        */
       _socket = socket(AF_INET, SOCK_STREAM, 0);
        /*
        Sockaddr_in contient les informations du serveur
        sin.family specifie le type d'adresse
        inet_addr specifie l'adresse  IP du serveur, ici localhost
        sin.port specifie le port en format reseau
        */
        _serverAddress.sin_family = AF_INET;
        _serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        _serverAddress.sin_port = htons(port);
        /*
        bind associe le socket à une adresse et un port spécifiques donc a mon ordi
        */
        bind(_socket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress));
        listen(_socket, 5);
    }
    else
        std::cout << "port not only composed by digit" << std::endl;
}

// const std::string getName() const
// {
//     return _name;
// }

// const int getSocket() const
// {
//     return _socket;
// }

Server::~Server()
{
    close(_socket);
}