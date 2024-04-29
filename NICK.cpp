#include "Server.hpp"

bool	Server::checkNickName(std::string to_check, int socket)
{
	std::string tmp_name;
	
	std::map<int, Client*>::iterator it = _clients.begin();
	int i = 0;
	while (it != _clients.end())
	{
		tmp_name = it->second->getNickName();
		if (to_check == tmp_name && socket != it->first)
		{
			return (false);
		}
		++it;
		i++;
	}
	return (true);
}

std::string intToString(int value)
{
    std::string result;
    bool negative = false;
    
    if (value < 0) {
        negative = true;
        value = -value;
    }
    
    do {
        result = char('0' + value % 10) + result;
        value /= 10;
    } while (value != 0);

    if (negative) {
        result = "-" + result;
    }
    
    return result;
}

void	Server::startingMsg(int socket)
{
	std::string total = "\n\x1b[34m╔═════════════════════════════╗\n\x1b[34m║\x1b[0m\x1b[36m  Bienvenue sur UTV Network\x1b[0m  \x1b[34m║\n\x1b[34m╚═════════════════════════════╝\x1b[0m\n        \x1b[1m\x1b[4mCreated by\x1b[0m :\n{ \x1b[1m\x1b[35mUaupetit\x1b[0m | \x1b[1m\x1b[31mTbourdea\x1b[0m | \x1b[1m\x1b[33mVgoret\x1b[0m }\n\n"; 
	std::cout << total << std::endl;

	// SEND WELCOME
	replyClient(total, socket);
	
	// SEND INFOS
	std::string infos = "\x1b[1mIl y a actuellement\x1b[33m " + intToString(getClientNumber()) + "\x1b[0m\x1b[1m utilisateurs connectés et \x1b[33m" + intToString(getChannelNumber()) + "\x1b[0m\x1b[1m channels de lancés !\x1b[0m\n";
	replyClient(infos, socket);

	// GET THE CLIENTS LIST
	std::map<int, Client *>::iterator it = _clients.begin();
	std::map<int, Client *>::iterator ite = _clients.end();
	std::string users_list;
	while (it != ite)
	{
		if (_clients[socket]->getNickName() == it->second->getNickName())
			users_list += "- " + it->second->getNickName() + " (\x1b[32mYOU\x1b[0m)\n";
		else
			users_list += "- " + it->second->getNickName() + "\n";
		++it;
	}

	// GET THE CHANNELS LIST
	std::map<std::string, Channel *>::iterator it2 = _channelLst.begin();
	std::map<std::string, Channel *>::iterator ite2 = _channelLst.end();
	std::string channels_list;
	int i = 0;
	while (it2 != ite2)
	{
		channels_list += "- #" + it2->second->getName() + "\n";
		i++;
		++it2;
	}
	if (i == 0)
		channels_list = "[ NONE 😭 ]";
	std::string infos2 = "\x1b[36mUsers actifs :\x1b[0m\n" + users_list + "\x1b[36mChannels actifs :\x1b[0m\n" + channels_list + "\n";
	std::cout << infos2 << std::endl;
	replyClient(infos2, socket);


	//BOT PART
	std::string check_mails = "\x1b[1m\x1b[32m📨 Tu as des messages non lu. (CTRL-N pour aller les voir !) 📨\x1b[0m\r\n";
	replyClient(check_mails, socket);

	std::string bjr = "Bonjour " + _clients[socket]->getNickName() + " ça baigne ? Je m'appelle \x1b[3m\x1b[36mBotimus_Maximus\x1b[0m\r\n";
	std::string help = "Tu peux m'envoyer \"HELP_ME\" si tu es perdu sur ce \x1b[3m\x1b[36mmagnifique\x1b[0m serveur ou \"COMMANDS\" si tu veux savoir ce dont je suis capable...\r\n";
	
	std::string blagues = "Tu peux aussi m'envoyer \"BLAGUES\" si tu veux te taper une grosse barre.";

	std::string bot_name = "Botimus_Maximus";
	std::cout << "Sending : " << PRIVMSG(bot_name, _clients[socket]->getNickName(), bjr) << std::endl;
	replyClient(PRIVMSG(bot_name, _clients[socket]->getNickName(), bjr), socket);

	std::cout << "Sending : " << PRIVMSG(bot_name, _clients[socket]->getNickName(), help) << std::endl;
	replyClient(PRIVMSG(bot_name, _clients[socket]->getNickName(), help), socket);
}

void	Server::nickCmd(std::string str, int socket)
{

	std::string cmd = str.substr(str.find(' ') + 1);	
	if (cmd[0] == '\0')
	{
		replyClient(ERR_NONICKNAMEGIVE(getClient(socket)->getNickName()), socket);
		return;
	}
	if (_clients[socket]->do_we_set_or_not() == true)
	{
		return ;
	}
	for (int i = 0; cmd[i]; i++)
	{
		if (!(((cmd[i] >= 'a' && cmd[i] <= 'z') || (cmd[i] >= 'A' && cmd[i] <= 'Z') || (cmd[i] >= '0' && cmd[i] <= '9') || cmd[i] == '_')) || (cmd.length() > 9))
		{
			std::string username;
			if (_clients[socket]->getUserName().empty())
				username = "<default_username>";
			else
				username = _clients[socket]->getUserName();
			std::string msg = ERR_ERRONEUSNICKNAME(username, cmd);
			std::cout << "SENDING : " << msg << std::endl;
			replyClient(msg, socket);
			return;
		}
	}
	std::map<int, Client*>::iterator it = _clients.find(socket);
	if (it != _clients.end())
	{
		// Attention a si meme nickname ya PROBLEMES ou si user essaye de changer de nickname
		std::string nickname = cmd;
		std::cout << nickname << std::endl;
		if (checkNickName(nickname, socket) == false)
		{
			replyClient(NICKNAMEINUSE_ERR(nickname), socket);
			return ;
		}
		else
		{
			if (_clients[socket]->getConnectedStatus() == false)
			{
				it->second->setNickName(cmd);
				it->second->updateStatus();
			}
			else
			{
				// ":nickname NICK new_nickname\r\n"
				std::string old = _clients[socket]->getNickName();
				std::string msg = ":" + old + " NICK " + nickname + "\r\n";
				std::cout << "SENDING ICI : " << msg << std::endl;
				replyClient(msg, socket);
				it->second->setNickName(cmd);
				//changeUsers(old, cmd);
				return ;
			}
		}
		if (it->second->getStatus() >= 4)
		{
			it->second->setNickName(cmd);
			std::cout << it->second->getNickName() << std::endl;
			std::string server_name = "localhost";
			std::string username = it->second->getUserName();
			std::string nickname = it->second->getNickName();
			if (_clients[socket]->getConnectedStatus() == false)
			{
				replyClient(WELCOME_MSG(server_name, nickname, username), socket);
				_nb_clients++;
				_clients[socket]->change_connected();
				startingMsg(socket);
			}
		}
	}
	else
		std::cerr << RED << "Client not found for socket: " << socket << std::endl;
}