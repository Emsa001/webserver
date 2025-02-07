#ifndef WEBSERV_HPP
#define WEBSERV_HPP

// system headers
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>

// c headers
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <poll.h>

// c++ headers
#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>

// project headers
#include "server.hpp"
#include "client.hpp"
#include "config.hpp"

#define PORT 8080
extern bool SERVER_STOP;

std::string cgi_execute(const std::string &scriptPath);
void handle_signal();

void trim(std::string &s);
void removeNewLines(std::string &s);
std::string getFirstWord(std::string const s);

#endif
