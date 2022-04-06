#include "../includes/Logic_User.hpp"

Logic_User::Logic_User()
{

}

Logic_User::~Logic_User()
{

}

void Logic_User::removeUserFromChannel(User *user)
{
	if (user->getChannel() != nullptr)
		user->getChannel()->removeUser(user);
}

void	Logic_User::appendMessage(string message, User *user)
{
	user->getMessage().append(message);
	user->getMessage().erase(user->getMessage().find_last_not_of(MSG_DELIMITER) + 1);
	user->getMessage().append("\n");
}

void	Logic_User::clearMessage(User *user)
{
	user->getMessage().clear();
}

void	Logic_User::sendMessage(User *from, User *to, string message)
{
	Logic_User::getReply(to, from->getPrefix() + " " + message);
}

void	Logic_User::doRegister(User *user)
{
	std:: string mewmessage = "";
	if (user->getDidEnter() && user->getNick() != "" && user->getUsername() != "") {
		Logic_User::getReply(user, mewmessage + RPL_WELCOME(user->getNick(), user->getUsername(), user->getHost()));
		user->setRegister(true);
	}
}

void	Logic_User::getReply(User *user, string message)
{
	std::string	finalMessage;

	finalMessage = message + MSG_DELIMITER;
	send(user->getSockFd(), finalMessage.c_str(), finalMessage.length(), 0);
}
