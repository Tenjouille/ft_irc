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
		Server server(av);
		try
		{
			int status;
		   // struct timeval timer;
			status = listen(server.getSocket(), 10);
			FD_ZERO(&server.getallSockets());
			FD_ZERO(&server.getreadFds());
			FD_SET(server.getSocket(), &server.getallSockets());
			server.setfdMax(server.getSocket());
			server.loop();
		}
		catch(const std::exception& e)
		{
			server.closeSockets();
			std::cerr << e.what() << '\n';
		}
	}
	else
		std::cout << "Bad arguments number" << std::endl;
}
