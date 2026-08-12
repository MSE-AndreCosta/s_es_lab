#ifndef MSGQ_H
#define MSGQ_H

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

typedef struct _msqg_node_t {
	void *data;
	struct _msqg_node_t *next;
} msgq_node_t;

typedef struct {
	msgq_node_t *head;
	msgq_node_t *tail;
	size_t count;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	bool shutdown;
} msgq_t;

/**
 * Initialize a message queue.
 * Must be called before using the queue.
 *
 * @param q Pointer to the queue to initialize
 */
void msgq_init(msgq_t *q);

/**
 * Push a message onto the queue.
 * Thread-safe and non-blocking.
 *
 * @param q Pointer to the queue
 * @param data Pointer to the message data (caller manages memory)
 * @return 0 if the message was queued successfuly or negative value on error
 */
int msgq_push(msgq_t *q, void *data);

/**
 * Pop a message from the queue.
 * Blocks until a message is available or the queue is shut down.
 * Thread-safe.
 *
 * @param q Pointer to the queue
 * @return Pointer to message data, or NULL if queue is shut down
 */
void *msgq_pop(msgq_t *q);

/**
 * Try to pop a message from the queue without blocking.
 * Returns immediately if the queue is empty.
 * Thread-safe.
 *
 * @param q Pointer to the queue
 * @return Pointer to message data, or NULL if queue is empty or shut down
 */
void *msgq_try_pop(msgq_t *q);

/**
 * Signal the queue to shut down.
 * Wakes all waiting threads. After this, msgq_pop() will return NULL.
 * The caller must join all threads before calling msgq_deinit().
 *
 * @param q Pointer to the queue
 */
void msgq_shutdown(msgq_t *q);

/**
 * Clean up queue resources.
 * Must be called after all threads have been joined.
 * Frees any remaining message nodes (but not the message data itself).
 *
 * @param q Pointer to the queue
 */
void msgq_deinit(msgq_t *q);

#endif /*MSGQ_H*/
