#include "Server.hpp"

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
            std::cout << "C'est bon !" << std::endl;
            server.setClientSocket(tmp);
            server.loop();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
        std::cout << "Bad arguments number" << std::endl;
}
