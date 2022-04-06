#include "../includes/Commands.hpp"
#include "../includes/Logic_Channel.hpp"

using namespace std;


Commands::Commands(Server *server) : _server(server) {
	commands[0] = "HELP";
	commands[1] = "NICK";
	commands[2] = "JOIN";
	commands[3] = "LEAVE";
	commands[4] = "WHO";
	commands[5] = "KICK";
	commands[6] = "LIST";
	commands[7] = "PASS";
	commands[8] = "ERROR";
	commands[9] = "QUIT";
	commands[10] = "USER";
	commands[11] = "NOTICE";
	commands[12] = "PRIVMSG";
}

Commands::~Commands() {}

void Commands::quit(User* sender){_server->removeUser(sender->getId());}

void Commands::help(User* sender)
{
	string helpDescription ="Команды:\n"
							"HELP - Показать подсказки с командами\n"
							"NICK <nick> - Изменить никнейм\n"
							"KICK <user> <#channel> - Удалить пользователя из канала\n"
							"JOIN <#channel> - Вступить в канал или создать его\n"
							"LEAVE #channel - Покинуть канал\n"
							"LIST - Показать каналы на этом сервере\n"
							"PASS <passwordtoserver> - Ввести пароль от сервера\n"
							"USER <username> <realname> - Используется в начале соединения для регистрации вас на сервере\n"
							"NOTICE <username> <message> — Отправить сообщение пользователю или серверу\n"
							"PRIVMSG <username> <message> — Отправить личное сообщение пользователю\n"
							"QUIT [ <Quit Message> ] — Выйти\n"
							"WHO <#channel> - Показать список пользователей, которые состоят в канале\n";
	Logic_User::getReply(sender, helpDescription);
}

void Commands::join(User* sender,deque<string> args)
{
	try
	{
		if (!sender->didRegister())
			throw ERR_RESTRICTED;
		if (args.size() < 1)
			throw;
		if (sender->getChannel() != nullptr)
			throw ERR_TOOMANYCHANNELS(sender->getChannel()->getName());
		if (!isAllowedChannelName(args[0]))
			throw ERR_BADCHANMASK(args[0]);

		Channel *channel = _server->getChannel(args[0]);
		if (!channel)
		{
			channel = _server->createChannel(args[0], sender);
		}
		sender->setChannel(channel);
		channel->addUser(sender);

		vector<User *>::iterator user;
		vector < User * > users = channel->getUsers();

		for (user = users.begin(); user != users.end(); user++)
		{
			Logic_User::getReply(*user, sender->getPrefix() + " " + "JOIN" + " #" + channel->getName());
		}
	} catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::nick(User* sender, deque<string> args)
{
	try
	{
		string _name = "NICK";

		if (args.size() < 1)
			throw ERR_NEEDMOREPARAMS(_name);

		string oldNick = sender->getNick();
		string newNick = args[0];

		if (_server->getUser(newNick))
			throw ERR_NICKNAMEINUSE(newNick);

		if (oldNick != "")
		{
			Logic_User::sendMessage(sender, sender, "NICK " + newNick.substr(0, 9));
		}
		sender->setNick(newNick);
		Logic_User::doRegister(sender);
	}catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::pass(User* sender, deque<string> args)
{
	try
	{
		string _name = "PASS";
		if (sender->didEnter())
			throw ERR_ALREADYREGISTRED;
		if (args.size() < 1)
			throw ERR_NEEDMOREPARAMS(_name);

		string password = args[0];
		if (!_server->checkPassword(password))
			throw ERR_PASSWDMISMATCH;

		Logic_User::sendMessage(sender, sender, "PASS Correct password");
		sender->setDidEnter(true);
		Logic_User::doRegister(sender);
	}catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::user(User *sender, deque <string> args)
{
	try
	{
		std::string _name = "USER";
		if (args.size() != 1)
			throw ERR_NEEDMOREPARAMS(_name);

		sender->setUsername(args[0]);
		Logic_User::doRegister(sender);
	}catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::leave(User *sender, deque <string> args)
{
	try
	{
		string _name = "LEAVE";
		if (!sender->didRegister())
			throw ERR_RESTRICTED;
		if (args.size() < 1)
			throw ERR_NEEDMOREPARAMS(_name);

		Channel *channel = _server->getChannel(args[0]);
		if (!channel)
			throw ERR_NOSUCHCHANNEL(args[0]);
		if (sender->getChannel() != channel)
			throw ERR_NOTONCHANNEL(args[0]);

		vector<User *>::iterator user;
		vector < User * > users = channel->getUsers();
		for (user = users.begin(); user != users.end(); user++)
		{
			Logic_User::getReply(*user, sender->getPrefix() + " " + _name + " #" + channel->getName());
		}
		Logic_User::removeUserFromChannel(sender);
		sender->setChannel(nullptr);
	} catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::who(User *sender, deque <string> args)
{
	try
	{
		string _name = "WHO";
		if (!sender->didRegister())
			throw ERR_RESTRICTED;
		if (args.size() < 1)
			throw ERR_NEEDMOREPARAMS(_name);
		Channel *channel = _server->getChannel(args[0]);
		if (channel == nullptr)
			throw ERR_NOSUCHCHANNEL(args[0]);

		vector < User * > users = channel->getUsers();
		Logic_User::getReply(sender, "Channel " + channel->getName() + " has " + std::to_string(users.size()) + " users");

		vector<User *>::iterator it;
		for (it = users.begin(); it != users.end(); it++)
		{
			if (channel->getAdmin() == *it)
				Logic_User::getReply(sender, "@" + (*it)->getName());
			else
				Logic_User::getReply(sender, (*it)->getName());
		}
	}catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::kick(User *sender, deque <string> args)
{
	try
	{
		string _name = "KICK";
		if (!sender->didRegister())
			throw ERR_RESTRICTED;
		if (args.size() < 2)
			throw ERR_NEEDMOREPARAMS(_name);
		if (!isAllowedChannelName(args[1]))
			throw ERR_BADCHANMASK(args[1]);

		Channel *channel = _server->getChannel(args[1]);

		if (channel == nullptr)
			throw ERR_NOSUCHCHANNEL(args[1]);
		if (channel->getAdmin() != sender)
			throw ERR_CHANOPRIVSNEEDED(channel->getName());
		if (!channel->getUser(sender->getNick()))
			throw ERR_NOTONCHANNEL(channel->getName());

		User *userToKick = channel->getUser(args[0]);

		if (userToKick == nullptr)
			throw ERR_USERNOTINCHANNEL(args[0], channel->getName());

		Logic_Channel::sendMessageToChannel(channel, sender, "kicked " + userToKick->getName(), _name);
		channel->removeUser(userToKick);
	}catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::list(User *sender)
{
	try
	{
		if (!sender->didRegister())
			throw ERR_RESTRICTED;

		vector < Channel * > channels = _server->getChannels();
		vector<Channel *>::iterator it;
		it = channels.begin();
		while( it != channels.end())
		{
			Logic_User::getReply(sender, RPL_LIST((*it)->getName(), to_string((*it)->getUsers().size())));
			it++;
		}
		Logic_User::getReply(sender, RPL_LISTEND);
	}catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::notice(User *sender, deque <string> args)
{
	try
	{
		if (!sender->didRegister())
			return;
		if (args.size() < 2)
			return;
		User *user = _server->getUser(args[0]);
		if (!user || user == sender)
			return;
		string message;
		deque<string>::iterator word;
		word = args.begin() + 1;
		while (word != args.end())
		{
			message += " " + *word;
			word++;
		}
		Logic_User::sendMessage(sender, user, "NOTICE " + user->getNick() + message);
	}catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

void Commands::privateMessage(User *sender, deque <string> args)
{
	try
	{
		string _name = "PRIVMSG";
		// Тут какой то багос с регистрациями
		// if (!sender->didRegister())
		// 	throw ERR_RESTRICTED;

		if (this->getUserFromArg(args) != nullptr)
		{
			User *user = this->getUserFromArg(args);
			Logic_User::sendMessage(sender, user, "PRIVMSG " + user->getNick() + " :" + this->makeString(args));
		} else if (this->getChannelFromArg(args) != nullptr)
		{
			Channel *tmp_channel = this->getChannelFromArg(args);
			std::string tmp;
			tmp = this->makeString(args);
			std::cout << tmp << std::endl;
			if (tmp_channel && !tmp.empty())
			{
				Logic_Channel::sendMessageToChannel(tmp_channel, sender, tmp, _name);
			}
		} else
			Logic_User::getReply(sender, "error nick or channel not found");
		this->clearArg(args);
	}catch (const char *message)
	{
		Logic_User::getReply(sender, string(message));
	} catch (string message)
	{
		Logic_User::getReply(sender, message);
	}
}

std::string Commands::makeString(deque <string> args) {

	std::string tmp;
	for (size_t i  = 1; i <= args.size(); i++)
	{
		if (!args[i].empty())
		{
			if (tmp.empty()) {
				tmp =  args[i];
			}
			else {
				tmp = tmp +  " " + args[i];
			}
		}
	}
	tmp.erase(tmp.find_last_not_of(MSG_DELIMITER) + 1);
	return tmp;
}

User *Commands::getUserFromArg(deque <string> args) {
	if (!args.empty())
	{
		User* user = _server->getUser(args[0]);
		if (user == nullptr)
			return  nullptr;
		else if(user)
			return  user;
	}
	return nullptr;
}

void Commands::clearArg(deque <string> args)
{
	args.erase(args.begin(), args.end());
}

Channel *Commands::getChannelFromArg(deque <string> args)
{
	if (!args.empty())
	{
		string getName = args[0].substr(1, args[0].size());
		Channel* channel = _server->getChannel(getName);
		if (channel == nullptr)
			return  nullptr;
		else if(channel)
			return  channel;
	}
	return nullptr;
}

void	Commands::processCommand(User* sender, deque<string> args) {
	string commandName = args[0];
	args.pop_front();

	if (commandName == "HELP")
		help(sender);
	else if (commandName == "NICK")
		nick(sender, args);
	else if (commandName == "JOIN")
		join(sender, args);
	else if (commandName == "LEAVE")
		leave(sender, args);
	else if(commandName == "WHO")
		who(sender, args);
	else if (commandName == "KICK")
		kick(sender, args);
	else if (commandName == "LIST")
		list(sender);
	else if (commandName == "PASS")
		pass(sender, args);
	else if (commandName == "QUIT")
		quit(sender);
	else if (commandName == "USER")
		user(sender, args);
	else if (commandName == "NOTICE")
		notice(sender, args);
	else if (commandName == "PRIVMSG")
		privateMessage(sender, args);
}

bool	Commands::isCommand(string data)
{
	int i = 0;
	while (i < 15)
		if (data == commands[i++])
			return true;
	return false;
}

deque<string> Commands::dataToArgs(string data) {
	stringstream	ssMsg(data);
	string			av;
	deque<string>	arguments;

	while (getline(ssMsg, av, ' '))
	{
		av.erase(av.find_last_not_of(MSG_DELIMITER) + 1);
		if (!av.empty())
			arguments.push_back(av);
	}
	return arguments;
}

void	Commands::processData(User *sender, string data) {
	deque<string> arguments = dataToArgs(data);
	if (!arguments.empty() && isCommand(arguments[0]))
	{
		if (sender->getName() != "")
			cout << "→ [" << sender->getName() << "] " << data;
		else
			cout << "→ [Незарегистрированный клиент]: " << data;
		processCommand(sender, arguments);
	}
}
