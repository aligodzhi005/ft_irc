#include "../includes/Logic_Channel.hpp"

Logic_Channel::Logic_Channel()
{

}

Logic_Channel::~Logic_Channel()
{

}

void	Logic_Channel::sendMessageToChannel(Channel *channel, User *sender, std::string message, std::string commandName) {
	std::vector<User *>::iterator user;

	for (user = channel->getUsers().begin(); user != channel->getUsers().end(); user++)
	{
		if (*user != sender)
			Logic_User::sendMessage(sender, *user, commandName + " #" + channel->getName() +  " :" + message + "\r\n");
	}
}