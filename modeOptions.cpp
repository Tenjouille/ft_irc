#include "Channel.hpp"

void	Channel::changeOperator(char sign, std::vector<std::string>& args)
{
	int 								socket = -1;
	std::map<int, Client*>::iterator	it = _clientslst.begin();

	for (;it != _clientslst.end(); it++)
		if (it->second->getNickName() == args[0])
			socket = it->first;
	if (sign == '+')
		newOperator(socket, it->second);
	else
		_operators.erase(socket);
	args.erase(args.begin());
}


void	Channel::changeInvit(char sign)
{
	if (sign == '+')
		_inviteonly = true;
	else
		_inviteonly = false;
}

void	Channel::changeTopic(char sign, std::vector<std::string>& args)
{
	if (sign == '+')
	{
		_topic = args[0];
		args.erase(args.begin());
	}
	else
		_topic = "";
}

void	Channel::changeKey(char sign, std::vector<std::string>& args)
{
	if (sign == '+')
	{
		_key = args[0];
		args.erase(args.begin());
	}
	else
		_key = "";
}

void	Channel::changeLimit(char sign, std::vector<std::string>& args)
{
	if (sign == '+')
	{
		_limit = atoi(args[0].c_str());
		args.erase(args.begin());
	}
	else
		_limit = 0;
}
