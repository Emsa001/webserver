#include <webserv.h>
#include <Server.h>

int testfun() {
	return 42;
}

void errexit(const char *err) {
	// fputs("Error: ", stderr);
	// fputs(err, stderr);
	// fputs("\n", stderr);
	exit(1);
}

void Server::run() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		errexit("can't create socket");
	struct sockaddr_in a;
	memset(&a, 0, sizeof(a));
	a.sin_family = AF_INET;
	a.sin_port = htons(80);
	if (bind(sock, (struct sockaddr *) &a, sizeof(a)) < 0)
		errexit("can't bind\n");
	if (listen(sock, 1) < 0)
		errexit("can't listen\n");

	struct sockaddr_in peeraddr;
	socklen_t peeraddr_len = sizeof(peeraddr);

	while (1) {
		int fd = accept(sock, (struct sockaddr *) &peeraddr, &peeraddr_len);
		if (fd < 0)
		errexit("accept() failed");
		printf("GET\n");
		send(fd, (void *) header, strlen(header), 0);
		send(fd, (void *) site, strlen(site), 0);
		close(fd);
	}
	close(sock);
}