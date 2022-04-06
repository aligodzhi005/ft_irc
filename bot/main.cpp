#include "Bot.hpp"

bool isWorking = true;

void signal_handler(int num)
{
	if (num == SIGQUIT)
		isWorking = false;
}

int main(int argc, char **argv)
{
	// setlocale (LC_ALL, "Russian");
	if (argc != 2 && argc != 3) {
		print_in_box("Ошибка запуска", RED BOLD);
		std::cout << std::endl;
		std::cerr << BOLD << "Правильный формат: " << argv[0] << " [Порт] [Пароль]" << CLEAR << std::endl;
		return 1;
	}

	if (signal(SIGQUIT, signal_handler) == SIG_ERR)
	{
		std::cout << "Ошибка при инициализации сигнала. Бот остановлен." << std::endl;
		isWorking = false;
	}
	else
	{
		std::cout << std::endl;
		print_ascii_image("../ascii/text-kenny");
		print_in_box("Бот запустился", GREEN BOLD);
		print_text("\nЭто бот Kenny!\nОн будет что-то делать, но когда-нибудь умрёт.\n", GREEN BOLD, true);
	}

	std::string pass;

	if (argc == 3)
		pass = argv[2];
	else
		pass = "";

	Bot bot(argv[1], pass);

	bot.connect_to_server();
	bot.auth();

	std::string msg;

	while(isWorking)
	{
		msg = bot.receiveMessage();
		if (msg.compare("") != 0)
			bot.parseMessage(msg);
	}
	return 0;
}
