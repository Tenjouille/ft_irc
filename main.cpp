#include "Server.hpp"

bool    is_valid_password(std::string password)
{
    if (password.empty())
    {
        std::cout << "Password Cannot Be Empty." << std::endl;
        return (false);
    }
    else if (password.length() < 5)
    {
        std::cout << "Password Needs To Be At Least 5 char long." << std::endl;
        return (false);
    }
    else 
    {
        size_t i = 0;
        while (i < password.length())
        {
            if (password[i] == ' ' || password[i] == '\n' || password[i] == '\r' || password[i] == '\t')
            {
                std::cout << "Forbidden Character in Password : '" << password[i] << "'." << std::endl;
                return (false);
            }
            i++;
        }
    }
    return (true);
}

int main(int ac, char **av)
{
    if (ac == 3)
    {
        try
        {
            std::string tmp_pass = av[2];
            if (is_valid_password(tmp_pass) == false)
            {
                std::cout << "Invalid Password" << std::endl;
                return (0);
            }
            std::cout << "Valide Password" << std::endl;

            Server server(av);
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
        std::cout << "Error : Bad Usage\nTry : ./ft_irc <port> <password>" << std::endl;
}
