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
#include <fstream>

// project headers
#include "Config.hpp"
#include "Server.hpp"


#endif
