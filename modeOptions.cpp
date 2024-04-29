#include "Channel.hpp"

void    Channel::changeOperator(char sign, std::vector<std::string>& args)
{
    int                                 socket = -1;
    std::map<int, Client*>::iterator    it = _clientslst.begin();

    if (!args.size())
        return ; // 461 ERR_NEEDMOREPARAMS
    for (;it != _clientslst.end(); it++)
        if (it->second->getNickName() == args[0])
            socket = it->first;
    if (it == _clientslst.end())
	{
		// std::string error = ERR_NOSUCHNICK(nickname_op, arg[0]);
		// std::string e_441 = ERR_USERONCHANNEL(nickname op, arg[0] , _name);
	    return ; // 401 ERR_NOSUCHNICK && 441 ERR_USERNOTINCHANNEL
	}
	for (std::map<int, Client*>::iterator it = _operators.begin();it != _operators.end(); it++)
            if (it->second->getNickName() == args[0])
                socket = -2;
    if (sign == '+' && socket != -2)
        newOperator(socket, it->second);
    else if (sign == '-' && socket == -2)
        _operators.erase(socket);
    args.erase(args.begin());
}


void    Channel::changeInvit(char sign)
{
    if (sign == '+')
        _inviteonly = true;
    else
        _inviteonly = false;
}

void    Channel::changeTopic(char sign)
{
    if (sign == '+')
        _topicStatus = 1;
    else
        _topicStatus = 0;
}

void    Channel::changeKey(char sign, std::vector<std::string>& args)
{
    if (sign == '+')
    {
        if (!args.size())
            return ; // 461 ERR_NEEDMOREPARAMS
        _key = args[0];
        args.erase(args.begin());
    }
    else
    {
        _key = "";
        if (args.size())
            args.erase(args.begin());
    }
}

void    Channel::changeLimit(char sign, std::vector<std::string>& args)
{
    if (sign == '+')
    {
        if (args.size() == 0)
            return ; // 461 ERR_NEEDMOREPARAMS
        if (atoi(args[0].c_str()) < 1 || strtod(args[0].c_str(), NULL) > 2147483647)
            return ; // (NO ERROR MSG FOUND)
        _limit = atoi(args[0].c_str());
        args.erase(args.begin());
    }
    else
        _limit = 0;
}