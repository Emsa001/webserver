#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Webserv.hpp"

class Logger {
public:
    static void info(const std::string& message) {
        std::cout << BOLD << BLUE500 << "[INFO] " << RESET << message << std::endl;
    }

    static void warning(const std::string& message) {
        std::cout << BOLD << YELLOW500 << "[WARNING] " << RESET << message << std::endl;
    }

    static void error(const std::string& message) {
        std::cout << BOLD << RED500 << "[ERROR] " << RESET << message << std::endl;
    }

    static void critical(const std::string& message) {
        std::cout << BOLD << RED900 << "[CRITICAL] " << RESET << message << std::endl;
    }

    static void debug(const std::string& message) {
        std::cout << BOLD << GREEN500 << "[DEBUG] " << RESET << message << std::endl;
    }


    // Custom Methods

    static void request(const std::string &message) {
        std:: cout << BOLD << PURPLE400 << "[REQUEST] " << RESET << message << std::endl;
    }

    static void clientConnect(const int clientId) {
        std::cout << BOLD << GREEN500 << "[CONNECT] " << RESET << "Client " << clientId << " has connected." << std::endl;
    }

    static void clientDisconnect(const int clientId) {
        std::cout << BOLD << ORANGE500 << "[DISCONNECT] " << RESET << "Client " << clientId << " has disconnected." << std::endl;
    }

    static void clientIdle(const int clientId) {
        std::cout << BOLD << ORANGE300 << "[IDLE] " << RESET << "Client " << clientId << " is idle." << std::endl;
    }

};

#endif