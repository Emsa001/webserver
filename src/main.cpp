#include "Webserv.hpp"

int main()
{
    std::string scripts[] = {
        "./www/cgi-bin/hello.py",
        "./www/cgi-bin/hello.php",
        "./www/cgi-bin/hello.sh"};

        std::cout << Cgi::execute(scripts[0]) << std::endl;

    return 0;
}
