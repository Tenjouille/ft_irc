#include "Server.hpp"

int main(int ac, char **av)
{
    if (ac == 2)
    {
        // try
        // {
            Server server(av[1]);
        // }
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }
        
    }
    else
        std::cout << "bad argument number" << std::endl;
}