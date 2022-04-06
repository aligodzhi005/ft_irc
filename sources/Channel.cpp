#include "../includes/Channel.hpp"
// #include "../includes/Logic_User.hpp"

Channel::Channel(std::string name, User* admin, Server *server) :
 _admin(admin), _server(server) {
	setName(name);
}

Channel::~Channel() {}


std::string			Channel::getName()const { return _name; }
User				*Channel::getAdmin()const { return _admin; }

void				Channel::setName(std::string name) {
	size_t maxNameLenght = 200;
	_name = name.substr(1, maxNameLenght);
}


void				Channel::addUser(User *newUser)
{
	_users.push_back(newUser);
}

void				Channel::removeUser(User *user)
{
	std::vector<User *>::iterator it;

	for (it = _users.begin(); it < _users.end(); it++)
	{
		if (*it == user) {
			_users.erase(it);
			(*it)->setChannel(nullptr);
			break ;
		}
	}
	if (_users.empty()) {
		_server->deleteChannel(this);
	}
}


void	Channel::sendMessageToChannel(User *sender, std::string message, std::string commandName) {
	std::vector<User *>::iterator user;

	for (user = _users.begin(); user != _users.end(); user++)
	{
		if (*user != sender)
			Logic_User::sendMessage(sender, *user, commandName + " #" + this->getName() +  " :" + message + "\r\n");
	}
}


User			*Channel::getUser(std::string userName)
{
	for (std::vector<User *>::iterator itUser = _users.begin(); itUser != _users.end(); itUser++)
	{
		std::string	curName = (*itUser)->getName();
		if (curName == userName)
			return *itUser;
	}
	return nullptr;
}

std::vector<User *> Channel::getUsers() const {
	return _users;
}

bool	isAllowedChannelName(std::string channelName) {
	if (channelName.empty()) {
		std::cout << "1";
		return false;
	}
	if (channelName[0] != '#' && channelName[0] != '&') {
		std::cout << "2";
		return false;
	}

	for (size_t i = 0; i < channelName.size(); i++) {
		if (channelName[i] == ' ' || channelName[i] == 7 || channelName[i] == ',') {
			std::cout << "3";
			return false;
		}
	}
	return true;
}
