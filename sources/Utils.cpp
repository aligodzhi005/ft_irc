#include "../includes/Utils.hpp"

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

void print_ascii_image(std::string filename)
{
	using namespace std;

	ifstream inf(filename);
	if (inf.is_open())
	{
		while (inf)
        {
            string strInput;
            getline(inf, strInput);
            print_text(strInput, GREEN, true);
        }
	}
}
