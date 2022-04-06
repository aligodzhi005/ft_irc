#ifndef FT_IRC1_BOT_HPP
#define FT_IRC1_BOT_HPP

#include <cmath>
#include <iostream>
#include <csignal>
#include <string>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>
#ifndef __APPLE__
	#include <wait.h>
#endif

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CLEAR "\033[0m"
#define BOLD  "\033[1m"
#define BUFFER 1024

class Bot
{
private:

	int			_serverPort;
	std::string _serverPass;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	bool		_isActive;
	int			_fd;
	struct sockaddr_in _addr;

	int			firstEnter;
	int			kennyLive;

	void sendData(std::string msg);
	std::string makeResponse(std::string text);
	int check_correct(std::string text);

public:
	Bot(std::string serverPort, std::string serverPass);
	~Bot(void);
	Bot(const Bot & other);
	Bot &operator=(const Bot & other);

	void connect_to_server(void);
	void auth(void);
	std::string receiveMessage(void);
	void parseMessage(std::string msg);
	void handler(std::string user, std::string text);
};

void	    	print_text(std::string text, std::string color, bool endline);
void        	print_ascii_image(std::string filename);
void	    	print_in_box(std::string text, std::string color);
std::string 	ascii_to_string(std::string filename);

#endif
