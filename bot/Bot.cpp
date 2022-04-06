#include "Bot.hpp"

void	print_text(std::string text, std::string color, bool endline)
{
    if (endline)
	    std::cout << color << text << std::endl << CLEAR;
    else
        std::cout << color << text << CLEAR;
}

void	print_in_box(std::string text, std::string color)
{
    std::cout << color << std::setfill('_') << std::setw(78) << CLEAR << std::endl;
    std::cout << color << "|" << std::setfill(' ') << std::setw(73) << "|" << CLEAR << std::endl;
    std::cout << color << "|" << std::setfill(' ') << std::setw(57) << text << std::setfill(' ') << std::setw(sizeof(text) + 5) << "|" << CLEAR << std::endl;
    std::cout << color << "|" << std::setfill(' ') << std::setw(73) << "|" << CLEAR << std::endl;
    std::cout << color << std::setfill('-') << std::setw(78) << CLEAR << std::endl;
}

void 	print_ascii_image(std::string filename)
{
	using namespace std;

	ifstream ken(filename);
	if (ken.is_open())
	{
		while (ken)
        {
            string strInput;
            getline(ken, strInput);
            print_text(strInput, GREEN, true);
        }
	}
}

std::string 	ascii_to_string(std::string filename)
{
	using namespace std;
	std::string cache = "\n";

	ifstream ats(filename);
	if (ats.is_open())
	{
		while (ats)
        {
            string strInput;
            getline(ats, strInput);
			cache.append(strInput);
			cache.append("\n");
        }
	}
	return cache;
}

Bot::Bot(std::string serverPort, std::string serverPass)
{
	std::stringstream ss;
	ss << serverPort;
	ss >> this->_serverPort;
	this->_serverPass = serverPass;
	this->_nickname = "kenny";
	this->_username = "kenny";
	this->_realname = "Kenny";
	this->firstEnter = 1;
	this->kennyLive = 1;
}

Bot::~Bot(void)
{
	close(_fd);
}

Bot::Bot(const Bot &other)
{
	if (this != &other)
	{
		_serverPort = other._serverPort;
		_serverPass = other._serverPass;
		_nickname = other._nickname;
		_username = other._username;
		_realname = other._realname;
	}
}

Bot &Bot::operator=(const Bot &other)
{
	if (this != &other)
	{
		_serverPort = other._serverPort;
		_serverPass = other._serverPass;
		_nickname = other._nickname;
		_username = other._username;
		_realname = other._realname;
	}
	return (*this);
}

void Bot::connect_to_server(void)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
	{
		std::cerr << "Не удаётся создать сокет." << std::endl;
		exit(1);
	}
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_serverPort);
	_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
	{
		std::cerr << "Не удаётся подключиться к серверу." << std::endl;
		exit(1);
	}
	fcntl(_fd, F_SETFL, O_NONBLOCK);
	_isActive = true;
}

void Bot::sendData(std::string msg)
{
	if (send(_fd, msg.c_str(), msg.length(), 0) < 0)
	{
		std::cerr << "Не удаётся отправить данные." << std::endl;
		exit(1);
	}
}

void Bot::auth(void)
{
	sendData("PASS " + _serverPass + "\r\n");
	usleep(100);
	sendData("NICK " + _nickname + "\r\n");
	usleep(100);
	sendData("USER " + _username + " 0 * " + _realname  + "\r\n");
	usleep(100);
}

std::string Bot::receiveMessage()
{
	char buf[BUFFER];
	std::string res = "";

	bzero(buf, strlen(buf));
	recv(_fd, buf, BUFFER, 0);
	res.append(buf);
	if (res == "")
		_isActive = false;
	bzero(buf, BUFFER);
	return res;
}

void Bot::handler(std::string user, std::string text)
{
	if (kennyLive == 1)
	{
		if (text.find("привет") != std::string::npos || text.find("здарова") != std::string::npos)
			sendData("PRIVMSG " + user + " " + "Здарова. Как сам, " + user + "?\r\n");
		else if (text.find("хорошо") != std::string::npos || text.find("норм") != std::string::npos)
			sendData("PRIVMSG " + user + " " + "Это супер! Что хочешь?\r\n");
		else if (text.find("300") != std::string::npos || text.find("погоду") != std::string::npos)
			sendData("PRIVMSG " + user + " " + "Я на это не поведусь." + "\n");
		else if (text.find("убить") != std::string::npos || text.find("умри") != std::string::npos)
		{
			kennyLive = 0;
			sendData("PRIVMSG " + user + " " + "Они убили Кенни — «Сволочи!»" + "\n");
		}
		else if (text.find("сколько") != std::string::npos)
			sendData("PRIVMSG " + user + " " + "Я умирал 78 раз." + "\n");
		else if (text[1] && firstEnter == 0)
			sendData("PRIVMSG " + user + " " + "Такую команду я не знаю.\0");
	}
	else
	{
		if (text.find("живи") != std::string::npos)
		{
			kennyLive = 1;
			sendData("PRIVMSG " + user + " " + "Я снова жив! Спасибо!" + "\n");
		}
		else if (text[1] && firstEnter == 0)
			sendData("PRIVMSG " + user + " " + "Это мама Кенни. Кенни мёртв.\n");
	}
}

void Bot::parseMessage(std::string msg)
{
	std::string user = "";
	std::string cmd = "";
	std::string text = "";

	if (firstEnter == 1)
		firstEnter = 0;
	else
	{
		// print_text("→ Сообщение: [" + msg + "]", GREEN, true);
		user = msg.substr(1, msg.find(" ") - 1);
		print_text("→ Ник пользователя: [" + user + "]", GREEN, true);
		cmd = msg.substr(msg.find(" ") + 1, msg.length());
		cmd = cmd.substr(0, cmd.find(" "));
		print_text("→ Команда: [" + cmd + "]", GREEN, true);
		text = msg.substr(msg.find(" ") + 1, msg.length());
		text = text.substr(text.find(" ") + 1, text.length());
		text = text.substr(text.find(" ") + 1, text.length());
		text = text.substr(1, text.rfind("\r") - 1);
		print_text("→ Обработал команду: [" + text + "]", BOLD, true);
		std::cout << std::endl;
	}

	handler(user, text);
}
