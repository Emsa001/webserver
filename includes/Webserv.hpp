#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#define PROJECT_NAME "webserv"

#include "Colors.hpp"

// system headers
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 

// network headers (for TCP)
#include <netinet/in.h>
#include <netinet/tcp.h>

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
#include <ctime>

#include <vector>
#include <map>
#include <stack>
#include <string>
#include <cstdlib>
#include <list>
#include <dirent.h>


#include "Aliases.hpp"

// project headers
#include "Utils.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Cgi.hpp"
#include "Logger.hpp"

extern bool g_stop;

// #include <iostream> // → Allows us to use std::cout and std::cerr for logging.
// #include <fstream> // → Used to read files (to serve static files).
// #include <sstream> // → Used for string manipulation (converting HTTP responses to strings).
// #include <string> // → Allows us to use std::string for text handling.
// #include <vector> // → Used for storing multiple sockets in poll().
// #include <cstring> // → Used for memset() to clear data structures.
// #include <sys/types.h> // → Required for networking functions.
// #include <sys/socket.h> // → Provides functions like socket(), bind(), listen(), accept(), etc.
// #include <netinet/in.h> // → Contains sockaddr_in (for IP addresses and ports).
// #include <unistd.h> // → Provides close() function for sockets.
// #include <fcntl.h> // → Used to set sockets as non-blocking.
// #include <poll.h> // → Allows us to use poll() for monitoring multiple clients.

#endif
