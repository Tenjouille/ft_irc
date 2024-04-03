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
            Client client;
            socklen_t client_addr_len = sizeof(struct sockaddr);
            struct sockaddr client_addr;
            int tmp = accept(server.getSocket(), (struct sockaddr *)&client_addr, &client_addr_len);
            client.setSocket(tmp);
          //  server.setClientSocket(tmp);
            bool sent = false;
            // int i = 0;
            while (true)
            {
                /*
                accepter demande de connexion client
                client addr = pointeur vers structur ou accept rempli les info de la socket client
                len = len de la struct
                */
                if (client.getSocket() != -1)
                {
                    //printf("New connection, Socket fd : %d, client fd : %d\n", server.getSocket(), server.getClientSocket());
                    char buffer[1024];
                  //  i++;
                    ssize_t bytes_received = recv(client.getSocket(), buffer, sizeof(buffer), 0);
                    buffer[bytes_received] = '\0'; 
                        std::cout << buffer << std::endl;
                    //parser buffer et foutre dans client
                    server.parser(buffer);
                    const char *buf = ":localhost 001 uaupetit :Welcome to the Internet Relay Network uaupetit!uaupetit\r\n";
                    
                    if (sent == false)
                    {   
                        ssize_t j = send(server.getClientSocket(), buf, strlen(buf), 0);
                        (void)j;
                        sent = true;
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
