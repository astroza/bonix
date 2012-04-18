/* BONIX - connect.c
 * 2008 - Felipe Astroza
 * Under GPLv3 (see LICENSE)
 */


#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <signal.h>

enum {
	TOO_FEW_ARGS = 1,
	SOCKERROR,
	PORTERROR, 
	CONNERROR,
	POLLERROR,
	WERROR
};

int main(int c, char **v)
{
	int sockfd, retval;
	unsigned int l_port;
	struct sockaddr_in so;
	char *error, buffer[1024];
	struct pollfd pfds[2];
	const int yes = 1;

	if(c < 3)
		return TOO_FEW_ARGS;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		return SOCKERROR;

	so.sin_family = AF_INET;
	so.sin_addr.s_addr = inet_addr(v[1]);
	l_port = strtoul(v[2], &error, 10);
	if(*error != 0 || l_port > 65535)
		return PORTERROR;

	so.sin_port = htons((uint16_t)l_port);

	if(connect(sockfd, (struct sockaddr *)&so, sizeof(so)) == -1)
		return CONNERROR;

	pfds[0].fd = 0;
	pfds[0].events = POLLIN|POLLHUP;

	ioctl(sockfd, FIONBIO, &yes);
	ioctl(0, FIONBIO, &yes);

	pfds[1].fd = sockfd;
	pfds[1].events = POLLIN|POLLERR|POLLHUP|POLLNVAL;

	signal(SIGPIPE, SIG_IGN);

	while(1) {
		if(poll(pfds, 2, -1) == -1)
			return POLLERROR;

		if(pfds[0].revents & POLLHUP)
			break;

		if(pfds[1].revents & (POLLERR|POLLHUP|POLLNVAL))
			break;

		if(pfds[0].revents & POLLIN) {
			ioctl(0, FIONREAD, &retval);
			if(retval == 0)
				break;

			while((retval = read(0, buffer, sizeof(buffer))) > 0)
				if(write(sockfd, buffer, retval) == -1)
					break;
		}

		if(pfds[1].revents & POLLIN) {
			ioctl(sockfd, FIONREAD, &retval);
			if(retval == 0)
				break;

			while((retval = read(sockfd, buffer, sizeof(buffer))) > 0)
				if(write(1, buffer, retval) == -1)
					return WERROR; /* Error fatal */
		}
	}

	close(sockfd);
	return 0;
}
