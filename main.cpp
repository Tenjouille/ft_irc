#include "Server.hpp"

// void    Server::execute(char *buffer)
// {
//     Commande    cmd(buffer);


// }

int main(int ac, char **av)
{
    if (ac == 2)
    {
        try
        {
            Server server(av[1]);
            socklen_t client_addr_len = sizeof(struct sockaddr);
            struct sockaddr client_addr;
            int tmp = accept(server.getSocket(), (struct sockaddr *)&client_addr, &client_addr_len);
            server.setClientSocket(tmp);

            int i = 0;
            while (true)
            {
                /*
                accepter demande de connexion client
                client addr = pointeur vers structur ou accept rempli les info de la socket client
                len = len de la struct
                */
                if (server.getClientSocket() != -1)
                {
                    //printf("New connection, Socket fd : %d, client fd : %d\n", server.getSocket(), server.getClientSocket());
                    char buffer[1024];
                    i++;
            ssize_t bytes_received = recv(server.getClientSocket(), buffer, sizeof(buffer), 0);
                    buffer[bytes_received] = '\0'; 
                        std::cout << buffer << std::endl;
                    //parser buffer et foutre dans client
                    server.parser(buffer);
                    const char *buf = ":localhost 001 uaupetit :Welcome to the Internet Relay Network uaupetit!uaupetit\r\n";
                    if (i == 1)
                    {   
                        ssize_t j = send(server.getClientSocket(), buf, strlen(buf), 0);
                        (void)j;
                    }
                    //bytes_received = 0;
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
        std::cout << "Bad arguments number" << std::endl;
}
