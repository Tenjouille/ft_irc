#include "Server.hpp"

int main(int ac, char **av)
{
    if (ac == 2)
    {
        try
        {
            Server server(av[1]);
            socklen_t client_addr_len = sizeof(struct sockaddr);
            while (true)
            {
                struct sockaddr_in client_addr;
                /*
                accepter demande de connexion client
                client addr = pointeur vers structur ou accept rempli les info de la socket client
                len = len de la struct
                */
                int tmp = accept(server.getSocket(), (struct sockaddr *)&client_addr, &client_addr_len);
                server.setClientSocket(tmp);
                if (server.getClientSocket() != -1)
                {
                    printf("New connection, Socket fd : %d, client fd : %d\n", server.getSocket(), server.getClientSocket());
                    // char buffer[1024];
                    // ssize_t bytes_received = recv(server.getClientSocket(), buffer, sizeof(buffer), 0);
                    // std::cout << buffer << std::endl;
                    // (void)bytes_received;
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
