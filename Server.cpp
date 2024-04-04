#include "Server.hpp"

Server::Server(char **av)
{
    size_t i;
    for(i = 0; isdigit(av[1][i]) != 0; i++)
    {}
    _password = av[2];
    std::cout << "Password Stocke : " << _password << std::endl;
    if (i == strlen(av[1]))
    {
        int port = atoi(av[1]);
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
        //listen(_socket, 5);
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
    Client* client = new Client(); // Instancier un nouveau client
    client->setSocket(tmp); // Définir le socket du nouveau client
    _clients.insert(std::make_pair(tmp, client));
}


// void    Server::loop()
// {
//     int i = 0;
//     while (true)
//     {
//         /*
//         accepter demande de connexion client
//         client addr = pointeur vers structur ou accept rempli les info de la socket client
//         len = len de la struct
//         */
//         if (_clientSocket != -1)
//         {
//             //printf("New connection, Socket fd : %d, client fd : %d\n", server.getSocket(), server._clientSocket);
//             char buffer[1024];
//             i++;
//             ssize_t bytes_received = recv(_clientSocket, buffer, sizeof(buffer), 0);
//             buffer[bytes_received] = '\0';
//             // std::cout << buffer << std::endl;
//             //parser buffer et foutre dans client
//             parser(buffer);
//             const char *buf = ":localhost 001 uaupetit :Welcome to the Internet Relay Network uaupetit!uaupetit\r\n";
//             // std::string buf = ":localhost 001 " + _clients[_clientSocket]. :Welcome to the Internet Relay Network uaupetit!uaupetit\r\n";
//             if (i == 1)
//             {   
//                 ssize_t j = send(_clientSocket, buf, strlen(buf), 0);
//                 (void)j;
//             }
//             //bytes_received = 0;
//         }
//     }
// }

void Server::read_data_from_socket(int socket, fd_set *all_sockets, int fd_max, int server_socket)
{
    char buffer[1024];
    char msg_to_send[1024];
    int bytes_read;
    int status;

    bytes_read = recv(socket, buffer, BUFSIZ, 0);
    std::cout << "BUFFER COMPLET : " << buffer << std::endl;
    for (int j = 0; j <= fd_max; j++)
    {
        if (FD_ISSET(j, all_sockets) && j != server_socket && j != socket)
        {
            status = send(j, msg_to_send, strlen(msg_to_send), 0);
        }
    }
    parser(buffer);
}

void Server::accept_new_connection(int server_socket, fd_set *all_sockets, int *fd_max)
{
    int client_fd;
    ///char msg_to_send[1024];
   // int status;

    client_fd = accept(server_socket, NULL, NULL);
    setClientSocket(client_fd); //**
    FD_SET(client_fd, all_sockets); // Ajoute la socket client à l'ensemble
    if (client_fd > *fd_max)
    {
        *fd_max = client_fd;
    }
    sendToClient("Erreur 464 :Password incorrect\n");
} 


void    Server::loop_bis(fd_set all_sockets, fd_set read_fds, int fd_max)
{
    struct timeval timer;
    int status;
    while (true)
    {
        read_fds = all_sockets;
        timer.tv_sec = 2;
        timer.tv_usec = 0;
        status = select(fd_max + 1, &read_fds, NULL, NULL, &timer);
        for (int i = 0; i <= fd_max; i++)
        {
            if (FD_ISSET(i, &read_fds) != 1)
            {
                // Le fd i n'est pas une socket à surveiller
                // on s'arrête là et on continue la boucle
                continue ;
            }
            // La socket est prête à être lue !
            if (i == _socket)
            {
                accept_new_connection(_socket, &all_sockets, &fd_max);
                //rajoute un socket client a all socket

            }
            else
            {
                read_data_from_socket(i, &all_sockets, fd_max, _socket);
                //sinon on lit le socket
            }
        }
    }
}

ssize_t Server::sendToClient(std::string to_send)
{
    //vide for now a remplir temporairement en attendant le nick et tout
    // std::cout << "CLIENT GETNAME VALUE : " << _clients.getName() << std::endl;
    // std::string buffer = _clients.getName() + to_send;
    // std::map<int, Client>::iterator it = *(_clients.begin());
    // std::cout << "MAP ICI : " << it->second << std::endl;
//   for (std::map<int, Clients *>::iterator it = _clients.begin(); it != clients.end(); ++it)
    // std::cout << it->first << " => " << it->second << '\n';

    ssize_t j = send(getClientSocket(), to_send.c_str(), to_send.length(), 0);
    return (j);
}

void    Server::defineCmd(std::string cmd, int start, int it)
{
    std::string locate;

    locate.append(cmd, start, it - start);
    if (locate.find("NICK") == 0)
    {
        std::cout << "!!!NICK COMMAND!!!" << std::endl;
     //   nickCmd (locate);
    }
    else if (locate.find("USER") == 0)
        std::cout << "!!!USER COMMAND!!!" << std::endl;
    else if (locate.find("MODE") == 0)
        std::cout << "!!!MODE COMMAND!!!" << std::endl;
    else if (locate.find("PASS") == 0)
    {
        std::cout << "!!!PASS COMMAND!!!" << std::endl;
        passCmd(locate);
    }
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

std::string Server::getServerPassword() const
{
    return _password;
}

void    Server::passCmd(std::string cmd)
{
    std::cout << "CMD :: " << cmd << std::endl;
    std::cout << "Password du serveur : " << getServerPassword() << std::endl;

    std::string server_pass = getServerPassword();

    //skip PASS
    int i = 0;
    while (cmd[i] != '\0' && cmd[i] != ' ')
        i++;
    i++;
    //now at the start of the entered password
    int start = i;
    std::string from_client = &cmd[start];
    std::cout << "TMP FROM STaRT : " << from_client << std::endl;
    std::cout << "AFTER SKIPPING PASS COMMAND : '" << &cmd[start] << "'" << std::endl;
    if (from_client.compare(server_pass) != 0)
    {
        std::cout << "ERRROOOOOR NOT THE SAME PASSWORD !!!" << std::endl;
        std::cout << server_pass << " vs " << from_client << std::endl;
        sendToClient("Erreur 464 :Password incorrect\n");
        //close la connection du coup
        return ;
    }
    else
    {
        //continue le processus de connection
        std::cout << "ALL GOOD SAME PASSWORD" << std::endl;
    }
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

// void Server::nickCmd(std::string str)
// {
//     std::string cmd = str.substr(str.find(' ') + 1);
//     std::cout << "cmd = '" << cmd << "'" << std::endl;
//     if (cmd.length() > 9)
//         printf("trop de char pour nick");
//     for (int i = 0; cmd[i]; i++)
//     {
//         if(!((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9')))
//         {
//             printf("nickname wrong input");
//         }
//     }
//     _client.setNickName(cmd);
//     std::cout << "client _name = '" << _client.getNickName() << "'" << std::endl;  
// }

Server::~Server()
{
    close(_socket);
}