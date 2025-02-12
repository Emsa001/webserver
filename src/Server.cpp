#include <stdexcept>
#include <iostream>

#include <webserv.h>
#include <Server.h>


int testfun() {
	return 42;
}

Server::Server(): _port(80) {
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0)
		throw std::runtime_error("socket()");
	struct sockaddr_in a;
	memset(&a, 0, sizeof(a));
	a.sin_family = AF_INET;
	a.sin_port = htons(_port);
	if (bind(_sockfd, (struct sockaddr *) &a, sizeof(a)) < 0)
		throw std::runtime_error("bind()");
	if (listen(_sockfd, 1) < 0)
		throw std::runtime_error("listen()");
}

void Server::run() {
	struct sockaddr_in peeraddr;
	socklen_t peeraddr_len = sizeof(peeraddr);
	std::cout << "server listening on port " << _port << std::endl;
	std::cout << "http://localhost:80" << std::endl;
	while (1) {
		int fd = accept(_sockfd, (struct sockaddr *) &peeraddr, &peeraddr_len);
		if (fd < 0)
			throw std::runtime_error("Couldn't accept() connection");
		printf("GET\n");
		send(fd, (void *) header, strlen(header), 0);
		send(fd, (void *) site, strlen(site), 0);
		close(fd);
	}
}

Server::~Server() {
	close(_sockfd);
}