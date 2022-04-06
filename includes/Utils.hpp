#ifndef UTILS_CLASS_HPP
# define UTILS_CLASS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CLEAR "\033[0m"
#define BOLD  "\033[1m"

void	    print_text(std::string text, std::string color, bool endline);
void        print_ascii_image(std::string filename);
void	    print_in_box(std::string text, std::string color);

#endif
