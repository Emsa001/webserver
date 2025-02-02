#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include "server.hpp"
#include "client.hpp"

#define PORT 8080
extern bool stop;

std::string cgi_execute(const std::string &scriptPath);
void handle_signal();

#endif
