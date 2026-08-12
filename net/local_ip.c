#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

char *net_fetch_local_ip(void)
{
	char *result = malloc(INET_ADDRSTRLEN);
	if (!result) {
		return NULL;
	}
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		goto err;
	}
	struct sockaddr_in dummy = {
		.sin_family = AF_INET,
		.sin_port = htons(53),
	};
	inet_pton(AF_INET, "8.8.8.8", &dummy.sin_addr);

	int ret = connect(fd, (struct sockaddr *)&dummy, sizeof(dummy));
	if (ret) {
		goto err;
	}

	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	ret = getsockname(fd, (struct sockaddr *)&addr, &addrlen);
	if (ret) {
		goto err;
	}
	inet_ntop(AF_INET, &addr.sin_addr, result, INET_ADDRSTRLEN);
	close(fd);
	return result;

err:
	free(result);
	if (fd >= 0) {
		close(fd);
	}
	return NULL;
}
