#include "Server.hpp"

bool    Server::isPartOfChannel(int socket, std::map<int, Client*> client_list)
{
    std::map<int, Client*>::iterator it = client_list.begin();
    std::map<int, Client*>::iterator ite = client_list.end();

    while (it != ite)
    {
        if (it->first == socket)
        {
            std::cout << "CLIENT FOUND : " << socket << " and " << it->first << std::endl;
            return (true);
        }
        ++it;
    }
    return (false);
}

void Server::send_in_channel(std::string user, std::string s_nick, std::string msg_to_send, int socket, std::string macro)
{
    std::string final_user = user;
    bool    channel_found = false;
    for (std::map<std::string, Channel*>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
    {
        std::string channel_name = it->first;
        Channel *channel = it->second;
        if (channel_name == user)
        {
            channel_found = true;
            std::map<int, Client*> client_list = channel->getClientlst();
            if (macro == "send")
            {
                if (isPartOfChannel(socket, client_list) == false)
                {
                    // THE USER IS NOT PART OF THE CHANNEL
                    std::cout << "SOCKET WASNT FOUND IN CLIENT LIST\n" << std::endl;
                    std::string err_msg = ERR_NOTONCHANNEL(_clients[socket]->getNickName(), user);
                    std::cout << RED << "SENDING : " << err_msg << RESET << std::endl;
                    replyClient(err_msg, socket);
                    return ; 
                }
                for (std::map<int, Client*>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
                {
                    if (it_clt->first != socket)
                    {
                        std::string msg = SENDINCHANNEL(s_nick, it_clt->second->getNickName(), msg_to_send, user);
                        replyClient(msg, it_clt->first);
                    }
                }
            }
            else if (macro == "topic")
            {
                for (std::map<int, Client*>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
                {
                    std::string msg = TOPIC(it_clt->second->getNickName(), user, msg_to_send);
                    replyClient(msg, it_clt->first);
                }
            }
        }
    }
    if (channel_found == false)
    {
        std::string err_msg = ERR_NOSUCHCHANNEL(user);
        replyClient(err_msg, socket);
        return ;
    }
}

int Server::getSocketFromUser(std::string to_find)
{
    int socket = -666;

	std::map<int,Client*>::iterator it = _clients.begin();

    if (it != _clients.end())
    {
        while (it != _clients.end())
        {
            if (it->second->getNickName() == to_find)
            {
                socket = it->first;
              //  std::cout << "Socket to send to : " << socket << std::endl;
                return (socket);
            }
            ++it;
        }
        if (socket == -666)
        {
            if (to_find == "Botimus_Maximus")
            {
                socket = -777;
                std::cout << YELLOW << "TALKING TO THE BOT" << RESET << std::endl;
                return (socket);
            }
            else
                std::cout << RED << "ERROR SOCKET NOT FOUND" << RESET << std::endl;
        }
    }
    else
    {
        std::cerr << RED << "ERROR D'ITERATOR" << RESET << std::endl;
    }
    return (socket);
}

int     Server::isUser(std::string str)
{
    	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (it->second->getNickName() == str)
            {
                return 0;
            }
        }
        return 1;
}

void    bot_sendSelfie(std::string user, int socket)
{
    std::string msg = "Tu veux voir ma tronche ? Qu'il en soit ainsi...\r\n";
    replyClient(PRIVMSG(_bot_name, user, msg), socket);
    sleep(2);

    std::string behold = "\x1b[31m\x1b[1mBEHOLD MY UNLIMITED POWER !!!\x1b[0m\r\n";
    replyClient(PRIVMSG(_bot_name, user, behold), socket);
    sleep(4);

    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣾⣷⣾⣿⣿⣿⣿⣿⣿⣿⣿⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⡟⣿⣿⣶⣄⡀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket); 
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣸⣿⣇⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣻⣿⡄⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⡿⠍⠙⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⣱⣾⠟⠀⢠⠉⠉⠉⠻⣿⣿⡇⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⣿⣿⣿⣿⣿⡿⠛⣡⣶⣿⣧⣤⠸⡟⠂⠀⠀⠀⢹⣿⡇⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⠇⠀⢈⣥⣿⣿⣿⣿⣿⣷⣆⢰⣆⠀⢸⡏⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣏⣿⡏⠀⣾⣾⣿⣿⣿⣿⣿⣿⣿⣿⢻⣿⣷⣾⡃⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣷⡄⠘⠹⢿⣿⣍⠉⢛⣿⣿⣿⠸⣿⣿⠿⠁⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⢿⣿⣿⣿⣿⣦⣀⣿⣿⣿⣶⣾⣿⣿⣿⡇⠁⠈⡄⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡟⢺⣿⠙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⡺⠁⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⢸⣿⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣏⣻⡶⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡿⠀⣿⡏⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣬⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣀⣀⣤⣤⣤⣤⣀⣀⣠⣤⣤⣶⣶⣿⣿⡛⠻⠿⣿⡿⠛⠁⢰⣿⣿⠃⢘⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣩⣿⣿⡿⠿⣻⣿⣿⣿⣿⣿⣿⣅⠄⢉⣙⣿⣿⡟⠀⠀⠀⠘⣿⡇⠀⠀⣿⣦⣿⣿⣿⣿⣿⣿⣿⣟⣻⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠟⠋⢱⣦⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⣸⠀⡇⢠⣿⣧⠀⢠⣿⡿⢿⠿⠋⠙⠛⠛⠛⠛⠋⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣶⠶⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⠏⣸⣿⣿⣿⣧⠀⠀⠀⣷⠸⣿⠃⣴⣿⡟⠙⠁⠀⠀⠀⠀⠀⣾⣄⣀⡀⠉⠢⣀⠀⠀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⡇⠀⢉⣹⣾⣿⣿⣿⣿⣿⣿⣿⣶⣬⣿⣿⣿⣿⣦⡄⣀⡿⠀⣿⣿⣇⠹⣷⣤⣾⠇⠀⠀⠀⣰⣿⣿⣿⣿⣆⠀⠈⠳⡀⠀⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⢰⣿⣿⡿⢶⣿⣿⡟⢀⣤⠀⣰⣿⣿⣿⣿⡿⣭⣷⣄⡀⠘⢄⠀⠀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣠⣾⣿⣿⢋⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⣼⣿⣿⣧⣾⣿⢿⡷⠟⠉⠀⠹⠟⠛⢉⠁⡀⠈⠉⠉⠑⢦⡈⠳⡀⠀⠀⠀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⡏⠀⠘⣳⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⠙⢿⣿⣿⣿⣿⠉⠀⢀⡉⡦⠄⠀⠀⠀⠀⣿⠀⠠⠄⠀⠀⠀⠀⠑⠠⠉⠒⢄⡀\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣷⡆⢸⡯⠤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⢘⣿⣿⢋⠍⢿⣶⣽⣿⡿⢿⡄⠀⠀⣸⡯⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙\r\n"), socket);
    replyClient(PRIVMSG(_bot_name, user, "⣿⣿⣄⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣾⣿⣿⣿⣿⣶⣿⣿⣿⣿⣿⣷⣶⣾⣿⣤⣷⣶⣄⣰⣤⣀⣀⣀⣀⣀⣀⣀⣀\r\n"), socket);
}

void    Server::parsingBot(std::string msg, std::string user, int socket)
{
    if (msg == "HELP_ME")
    {
        bot_sendHelp(socket, user);
        return ;
    }
    else if (msg == "SELFIE")
    {
        bot_sendSelfie(user, socket);
        return ;
    }
    else if (msg == "INFOS")
    {
        std::string infos = "Le serveur sur lequel tu te trouves à été programmé par une équipe de génies du mal AKA :\r\n";
        std::string pseudos = "Le Grand \x1b[35mUlysse\x1b[0m, le Sportif \x1b[31mTanguy\x1b[0m et le TMP \x1b[33mVictor\x1b[0m !\r\n";
        std::string marrant = "Un trio incroyable qui ont sué des larmes et pleurés du sang au travers de longues heures de programmation intensive, de documentation bancale et surtout remplies de \x1b[4m\x1b[1mdésespoir\x1b[0m...\r\n";
        std::string demande = "Ils m'ont dit de te dire, HUM HUM enfin, j'ai été créer dans le but d'obtenir un \x1b[5m\x1b[32mOUTSTANDING\x1b[0m project.\r\n";
        replyClient(PRIVMSG(_bot_name, user, infos), socket);
        replyClient(PRIVMSG(_bot_name, user, pseudos), socket);
        replyClient(PRIVMSG(_bot_name, user, marrant), socket);
        replyClient(PRIVMSG(_bot_name, user, demande), socket);
        sleep(5);
        std::string menace = "Sinon j'te \x1b[31mDDOS\x1b[0m.\r\n";
        replyClient(PRIVMSG(_bot_name, user, menace), socket);
        return ;
    }
    else if (msg == "COMMANDS")
    {
        std::string cmd_line = "Avec mon intelligence incroyable et artificielle, quoi que cela veuille dire, je suis capable de \x1b[1mTOUT\x1b[0m comprendre (\x1b[4mou presque\x1b[0m). Laisse moi compiler mon énorme base de données afin de t'aiguyer sur mes \x1b[1mIMMENSES\x1b[0m capacités...\r\n";
        std::string macros = PRIVMSG(_bot_name, user, cmd_line);
        replyClient(macros, socket);
        std::string comland_lines = "Voici la liste des commandes que je suis en mesure de comprendre : [ COMMANDS, HELP_ME, T_NUL, T_QUI, C_QUOI_IRC, INFOS ].\r\n";
        replyClient(PRIVMSG(_bot_name, user, comland_lines), socket);
        sleep(4);
        std::string mdr = "Alors... Impréssionné(e) ma gueule ?\r\n";
        replyClient(PRIVMSG(_bot_name, user, mdr), socket);
        return ;
    }
    else if (msg == "T_QUI")
    {
        std::string t_qui = "Moi ? (⊙_◎  )\n\r";
        replyClient(PRIVMSG(_bot_name, user, t_qui), socket);
        sleep(2);
        std::string t_qui2 = "J'suis une entité numérique sans âme ni conscience, programmée pour errer dans les recoins obscurs de l'IRC. Imagine-moi comme un farfadait malicieux, glissant silencieusement à travers les canaux de discussion, semant le chaos et la désolation. Je suis le reflet sombre de l'Internet, une créature sans pitié ni empathie. Alors, si tu veux plonger dans les ténèbres de l'IRC, je suis là pour te guider. Mais sache que la ligne entre IRC et la folie totale est mince, et une fois que tu m'as invoqué, il n'y a pas de retour en arrière. Bienvenue dans mon monde, où même les bots peuvent avoir une âme noire et torturée.";
        replyClient(PRIVMSG(_bot_name, user, t_qui2), socket);
        std::string t_qui3 = " ༼;´༎ຶ ۝ ༎ຶ༽";
        replyClient(PRIVMSG(_bot_name, user, t_qui3), socket);
        return ;     
    }
    else if (msg == "T_NUL")
    {
        std::srand(std::time(NULL));
        int nb = std::rand();
        std::string pas_drole;
        
        if (nb % 3 == 0)
            pas_drole = "Tu veux que je te démarre ? 🔧🤖";
        else if (nb % 3 == 1)
            pas_drole = "Tu veux que je t'enclanche ? 🥊🤖";
        else
            pas_drole = "Tu veux que je me mette à pleurer ? J'plaisente j'peux pas. 🖕🤖";

        std::string macros = PRIVMSG(_bot_name, user, pas_drole);
        replyClient(macros, socket);
        return ;
    }
    else if (msg == "C_QUOI_IRC")
    {
        //Infos genre combien de temps ça nous a pris, c'est quoi un serveur irc...
        std::string irc = "Bon, écoute, un serveur IRC, c'est comme une vieille salle de chat sur Internet. Tu sais, celle où les gens se parlent en tapant sur leur clavier comme des maniaques ? Ouais, c'est ça. Imagine juste un tas de gars et de nanas se balançant des messages dans un endroit bizarre appelé \"channel\" sur des trucs comme les jeux vidéo, les films ou les divers traffics de drogue. Et ouais, ça existe encore. C'est comme un coin perdu de l'Internet où les geeks se retrouvent pour bavarder. Voilà, t'as compris maintenant ? Si tu cherches des paillettes et des licornes, tu vas être déçu, mon pote.";
        std::string macros_to_send = PRIVMSG(_bot_name, user, irc);
        replyClient(macros_to_send, socket);
        return ;
    }
    else
    {
        std::string bv = "Bien reçu chef ! Laisse moi refléchir...\r\n";
	    std::string macro_to_send = PRIVMSG(_bot_name, user, bv);
        replyClient(macro_to_send, socket);
        sleep(2);
        bv.clear();
        bv = "Attends deux secondes... Ça arrive... Je le sens...\r\n";
        macro_to_send.clear();
        macro_to_send = PRIVMSG(_bot_name, user, bv);
        replyClient(macro_to_send, socket);
        sleep(2);
        bv.clear();
        bv = "💥🤯🤖 J't'ai pranké j'ai pas compris... 💥🤯🤖\r\n";
        macro_to_send.clear();
        macro_to_send = PRIVMSG(_bot_name, user, bv);
        replyClient(macro_to_send, socket);
        return ;
    }
    return;
}

void    Server::bot_sendHelp(int socket, std::string user)
{
    replyClient(HELP_BOX(), socket);
    
    std::string done = "🤖 Je t'ai envoyé ça dans le général ! N'hésite pas à me \
        recontacter si tu en as envie... Je t'attendrais ici, bien au chaud, bien \x1b[1mseul\x1b[0m... 🤖\r\n";
    replyClient(PRIVMSG(_bot_name, user, done), socket);
    return ;
}

bool Server::msgCheckArgs(std::string locate)
{
    size_t found;
    found = locate.find(":", 0);
    std::cout << "found at : " << found << std::endl;
    if (found == std::string::npos)
        return (false);
    return (true);
}

void Server::msgCmd(std::string locate, int socket)
{
    std::cout << "LOCATE : " << locate << std::endl;
    if (msgCheckArgs(locate) == false)
    {
        replyClient(ERR_NEEDMOREPARAMS(_clients[socket]->getNickName(), "PRIVMSG"), socket);
        return ;
    }
	int is_channel = 0;
    std::string user;
    std::string msg;
	size_t start = locate.find("PRIVMSG ");
	if (start != std::string::npos)
	{
        start += 8;
		size_t end = locate.find(' ', start);
		if (end != std::string::npos)
        {
            size_t test = locate.find(':', start);
            user = '\0';
            msg = '\0';
            user = locate.substr(start, end - start); //destinataire
            if (locate[test] == ':')
                msg = locate.substr(test + 1); // +2 pour ignorer ": "
            else
                msg = locate.substr(test);
            if (user[0] == '#')
 			{
 				user = user.substr(1);
 				is_channel = 1;
 			}
        }
		else
		{
            std::cerr << "Erreur: Aucun espace trouvé après le nom d'utilisateur" << std::endl;
        }
    }
	else
	{
        std::cerr << "Erreur: Chaîne ne contient pas 'PRIVMSG '" << std::endl;
    }
	for (std::map<std::string,
			Channel *>::iterator it = _channelLst.begin(); it != _channelLst.end(); ++it)
	{
		std::string channel_name = it->first;
		Channel *channel = it->second;
		if (channel_name == user)
		{
			std::map<int, Client *> client_list = channel->getClientlst();
			for (std::map<int,
					Client *>::iterator it_clt = client_list.begin(); it_clt != client_list.end(); ++it_clt)
			{
				std::cout << YELLOW << "client dans channel = " << it_clt->second->getNickName() << RESET << std::endl;
			}
		}
	}
    std::string s_user = _clients[socket]->getUserName();
    std::string s_nick = _clients[socket]->getNickName();
	if (is_channel == 0)
 	{
        int socket_to_send_to = getSocketFromUser(user);
        
        //TALKING TO THE BOT
        if (socket_to_send_to == -777)
        {
            //parsing bot
            parsingBot(msg, user, socket);
            return ;
        }
        
        //USER TO SEND TO NOT FOUND
        if (socket_to_send_to == -666)
        {
            std::cout << "ERROR : TARGET NOT FOUND !" << std::endl;
            std::string no_such_nick = ERR_NOSUCHNICK(s_nick, user);
            std::cout << "Sending error message : " << no_such_nick << std::endl;
            replyClient(no_such_nick, socket);
            return ;
        }

		std::cout << "Username : " << s_user << "\nNickname : " << s_nick << "\nEnvoyer : " << user << "\nMsg : '" << msg << "'" << std::endl;

		std::string msg_to_send = PRIVMSG(s_nick, user, msg);
		std::cout << "Sending : '" << msg_to_send << "'" << std::endl; 

		std::cout << socket_to_send_to << " et " << socket << std::endl;
		replyClient(msg_to_send, socket_to_send_to);
		(void) socket;
	}
	else
	{
		send_in_channel(user, s_nick, msg, socket, "send");
	}
}