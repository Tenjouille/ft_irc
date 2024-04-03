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

int Server::getClientSocket() const
{
    return _clientSocket;
}

void Server::setClientSocket(int tmp)
{
    _clientSocket = tmp;

}

void    Server::newClient(int socket, struct sockaddr client_addr)
{
    _clients.insert(std::pair<int, Client*>(socket, new Client(client_addr)));
}

void    Server::defineCmd(std::string cmd, int start, int it)
{
    std::string locate;

    locate.append(cmd, start, it - start);
    std::cout << locate << std::endl;
    if (locate.find("NICK") == 0)
        std::cout << "!!!NICK COMMAND!!!" << std::endl;
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
    else if (locate.find("QUIT") == 0)
        std::cout << "!!!QUIT COMMAND!!!" << std::endl;
    else
        std::cout << "???ERROR UNKNOW COMMAND???" << std::endl;
}

void    Server::parser(char *buffer)
{
    //tant que le buffer n'est pas /r/n
    // int         j = 0;
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

void    Server::loop()
{
    int i = 0;
    while (true)
    {
        /*
        accepter demande de connexion client
        client addr = pointeur vers structur ou accept rempli les info de la socket client
        len = len de la struct
        */
        if (_clientSocket != -1)
        {
            //printf("New connection, Socket fd : %d, client fd : %d\n", server.getSocket(), server._clientSocket);
            char buffer[1024];
            i++;
            ssize_t bytes_received = recv(_clientSocket, buffer, sizeof(buffer), 0);
            buffer[bytes_received] = '\0';
            // std::cout << buffer << std::endl;
            //parser buffer et foutre dans client
            parser(buffer);
            const char *buf = ":localhost 001 uaupetit :Welcome to the Internet Relay Network uaupetit!uaupetit\r\n";
            if (i == 1)
            {   
                ssize_t j = send(_clientSocket, buf, strlen(buf), 0);
                (void)j;
            }
            //bytes_received = 0;
        }
    }
}

Server::~Server()
{
    close(_socket);
}