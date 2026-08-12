
/**
 * @file net_udp.h
 * @brief Minimal UDP socket interface for sending and receiving binary frames.
 *
 * lib_udp wraps POSIX sockets into a small opaque-handle API for transmitting
 * and receiving fixed or variable-length binary frames over UDP. *
 *
 * The socket is created in connected mode (via @ref udp_create), meaning
 * @ref udp_send_frame and @ref udp_recv_frame implicitly target the address
 * given at creation time. No session or reliability layer is provided; packet
 * ordering, loss, and deduplication are the caller's responsibility.
 *
 */

#ifndef LIB_UDP_H
#define LIB_UDP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

/**
 * @brief Opaque handle representing an open UDP socket.
 */
typedef struct udp_socket udp_socket_t;

/**
 * @brief Creates a UDP socket connected to the given remote host and port for sending.
 *
 * No bind is performed — the kernel assigns an ephemeral source port.
 * Use @ref udp_send_frame to transmit data.
 *
 * @param host  Remote hostname or IP address string (e.g. "192.168.1.10").
 * @param port  Remote UDP port in host byte order.
 *
 * @return Pointer to a newly allocated socket handle on success, NULL on failure.
 */
udp_socket_t *udp_create_sender(const char *host, uint16_t port);

/**
 * @brief Creates a UDP socket bound to the given local port for receiving.
 *
 * The socket is bound to INADDR_ANY on the specified port, so it will
 * receive datagrams sent to that port from any host or interface.
 *
 * @param port  Local UDP port to bind to, in host byte order.
 *
 * @return Pointer to a newly allocated socket handle on success, NULL on failure.
 */
udp_socket_t *udp_create_receiver(uint16_t port);

/**
 * @brief Closes the socket and frees all resources associated with the handle.
 * @param sock  Handle to destroy. No-op if NULL.
 */
void udp_delete(udp_socket_t *sock);

/**
 * @brief Sends a single frame to the specified remote address.
 *
 * @param sock  Socket handle returned by @ref udp_create.
 * @param buf   Pointer to the data to send.
 * @param len   Number of bytes to send. Must not exceed the platform UDP
 *              payload limit (~65507 bytes); in practice keep under 1400
 *              bytes to stay within a typical Ethernet MTU.
 *
 * @return Number of bytes sent on success, negative value on error.
 */
ssize_t udp_send_frame(udp_socket_t *sock, const void *buf, size_t len);

/**
 * @brief Receives a single frame from the remote address (blocking).
 *
 * @param sock Socket handle returned by @ref udp_create.
 * @param buf  Destination buffer for the received payload.
 * @param len  Capacity of @p buf in bytes.
 *
 * @return Number of bytes received on success.
 * @return  0 on timeout (if a timeout was set via @ref udp_set_timeout) or negative value on error.
 */
ssize_t udp_recv_frame(udp_socket_t *sock, void *buf, size_t len);

/**
 * @brief Sets a receive timeout on the socket.
 *
 * After this call, @ref udp_recv_frame will return 0 if no datagram arrives
 * within @p timeout_ms milliseconds, rather than blocking indefinitely.
 * Pass 0 to disable the timeout and restore blocking behaviour.
 *
 * @param sock        Socket handle returned by @ref udp_create.
 * @param timeout_ms  Receive timeout in milliseconds, or 0 to disable.
 *
 * @return 0 on success or negative value on error.
 */
int udp_set_timeout(udp_socket_t *sock, uint32_t timeout_ms);

/**
 * @brief Returns the underlying file descriptor of the socket.
 *
 * Useful for integrating with @c select / @c poll loops or passing the socket
 * to other POSIX APIs without exposing the internals of @ref udp_socket_t.
 *
 * @param[in] sock  Socket handle returned by @ref udp_create.
 *
 * @return The socket file descriptor (>= 0) or negative value on error.
 */
int udp_get_fd(const udp_socket_t *sock);

int udp_set_broadcast(udp_socket_t *sock, bool enable);

#endif /* LIB_UDP_H */
