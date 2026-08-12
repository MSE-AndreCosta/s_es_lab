#include "net_udp.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

struct udp_socket {
	int fd; /**< POSIX socket file descriptor. */
	char *host;
	uint16_t port;
};

udp_socket_t *udp_create_receiver(uint16_t port)
{
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd < 0) {
		return NULL;
	}
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr.s_addr = htonl(INADDR_ANY),
	};
	int err = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (err) {
		close(fd);
		return NULL;
	}
	udp_socket_t *sock = malloc(sizeof(*sock));
	if (!sock) {
		close(fd);
		return NULL;
	}
	sock->fd = fd;
	sock->host = NULL;
	sock->port = port;
	return sock;
}

udp_socket_t *udp_create_sender(const char *host, uint16_t port)
{
	if (!host) {
		return NULL;
	}
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd < 0) {
		return NULL;
	}
	udp_socket_t *sock = malloc(sizeof(*sock));
	if (!sock) {
		close(fd);
		return NULL;
	}
	sock->fd = fd;
	sock->host = strdup(host);
	if (!sock->host) {
		close(fd);
		free(sock);
		return NULL;
	}
	sock->port = port;
	return sock;
}

int udp_set_broadcast(udp_socket_t *sock, bool enable)
{
	if (!sock) {
		return -EINVAL;
	}
	int en = enable ? 1 : 0;
	return setsockopt(sock->fd, SOL_SOCKET, SO_BROADCAST, &en, sizeof(en));
}

void udp_delete(udp_socket_t *sock)
{
	if (!sock) {
		return;
	}
	free(sock->host);
	sock->host = NULL;
	close(sock->fd);
	free(sock);
}

ssize_t udp_send_frame(udp_socket_t *sock, const void *buf, size_t len)
{
	if (!sock || !buf) {
		return -EINVAL;
	}
	if (!sock->host) {
		return -EINVAL;
	}
	struct sockaddr_in dst = {
		.sin_family = AF_INET,
		.sin_port = htons(sock->port),
		.sin_addr.s_addr = inet_addr(sock->host),
	};
	return sendto(sock->fd, buf, len, 0, (struct sockaddr *)&dst, sizeof(dst));
}

ssize_t udp_recv_frame(udp_socket_t *sock, void *buf, size_t len)
{
	if (!sock || !buf) {
		return -EINVAL;
	}
	ssize_t n = recv(sock->fd, buf, len, 0);
	if (n < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return 0;
		}
	}
	return n;
}

int udp_set_timeout(udp_socket_t *sock, uint32_t timeout_ms)
{
	if (!sock) {
		return -EINVAL;
	}
	struct timeval tv = {
		.tv_sec = timeout_ms / 1000,
		.tv_usec = (timeout_ms % 1000) * 1000,
	};
	return setsockopt(sock->fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

int udp_get_fd(const udp_socket_t *sock)
{
	if (!sock) {
		return -EINVAL;
	}
	return sock->fd;
}
