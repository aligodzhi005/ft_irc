#include "../includes/ircserv.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"

int main(int argc, char **argv)
{
    try {
        if (argc < 2 || argc > 3)
            throw std::runtime_error("Некорректный ввод. Для запуска введите команду в формате: ircserv [host:port:password] <port> <password>");
        if (argc == 3)
        {
            Server ircserv("127.0.0.1", argv[1], argv[2]);
            ircserv.start();
        }
        else
        {
            std::vector<std::string> result;
            std::stringstream ss(argv[1]);
            std::string token;

            while (std::getline(ss, token, ':'))
                if (!token.empty())
                    result.push_back(token);

            if (result.size() != 3)
                throw std::runtime_error("Для запуска введите команду в формате: ircserv [host:port:password] <port> <password>");
            Server ircserv(result[0], result[1], result[2]);
            ircserv.start();
        }
    } catch (const std::exception& e) {
        print_in_box("Ошибка сервера", RED BOLD);
        std::cout << std::endl;
        print_text(e.what(), RED, true);
        std::cout << std::endl;
    }
    return 0;
}
