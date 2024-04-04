#include "Server.hpp"

// int main(int ac, char **av)
// {
//     if (ac == 2)
//     {
//         try
//         {
//             Server server(av[1]);
//             socklen_t client_addr_len = sizeof(struct sockaddr);
//             struct sockaddr client_addr;
//             int tmp = accept(server.getSocket(), (struct sockaddr *)&client_addr, &client_addr_len);
//             server.setClientSocket(tmp);
//             fcntl(server.getSocket(), F_SETFL, O_NONBLOCK);
//             server.loop();
//             // client.setSocket(tmp);
//           //  server.setClientSocket(tmp);
//             // bool sent = false;
//             // // int i = 0;
//             // while (true)
//             // {
//             //     /*
//             //     accepter demande de connexion client
//             //     client addr = pointeur vers structur ou accept rempli les info de la socket client
//             //     len = len de la struct
//             //     */
//             //     if (client.getSocket() != -1)
//             //     {
//             //         //printf("New connection, Socket fd : %d, client fd : %d\n", server.getSocket(), server.getClientSocket());
//             //         char buffer[1024];
//             //       //  i++;
//             //         ssize_t bytes_received = recv(client.getSocket(), buffer, sizeof(buffer), 0);
//             //         buffer[bytes_received] = '\0'; 
//             //             std::cout << buffer << std::endl;
//             //         //parser buffer et foutre dans client
//             //         server.parser(buffer);
//             //         const char *buf = ":localhost 001 uaupetit :Welcome to the Internet Relay Network uaupetit!uaupetit\r\n";
                    
//             //         if (sent == false)
//             //         {   
//             //             ssize_t j = send(server.getClientSocket(), buf, strlen(buf), 0);
//             //             (void)j;
//             //             sent = true;
//             //         }
//             //         //bytes_received = 0;
//             //     }
//             // }
//         }
//         catch(const std::exception& e)
//         {
//             std::cerr << e.what() << '\n';
//         }
//     }
//     else
//         std::cout << "Bad arguments number" << std::endl;
// }


int main(int ac, char **av)
{
    if (ac == 3)
    {
        try
        {
            int status;
            Server server(av);
            fd_set all_sockets; // Ensemble de toutes les sockets du serveur
            fd_set read_fds;    // Ensemble temporaire pour select()
            int fd_max;         // Descripteur de la plus grande socket
           // struct timeval timer;
            status = listen(server.getSocket(), 10);
            // Préparation des ensembles de sockets pour select()
            FD_ZERO(&all_sockets);
            FD_ZERO(&read_fds);
            FD_SET(server.getSocket(), &all_sockets); // Ajout de la socket principale à l'ensemble
            fd_max = server.getSocket(); // Le descripteur le plus grand est forcément celui de notre seule socket
            server.loop_bis(all_sockets, read_fds, fd_max);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
        std::cout << "Bad arguments number" << std::endl;
}