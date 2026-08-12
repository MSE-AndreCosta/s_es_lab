#include <msgq/msgq.h>

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <asm-generic/errno-base.h>

void msgq_init(msgq_t *q)
{
	if (!q) {
		return;
	}
	memset(q, 0, sizeof(*q));
	q->shutdown = false;
	pthread_mutex_init(&q->mutex, NULL);
	pthread_cond_init(&q->cond, NULL);
}

int msgq_push(msgq_t *q, void *data)
{
	if (!q) {
		return -EINVAL;
	}
	msgq_node_t *node = malloc(sizeof(*node));
	if (!node) {
		return -ENOMEM;
	}
	node->data = data;
	node->next = NULL;
	int err = pthread_mutex_lock(&q->mutex);
	if (err) {
		return err;
	}
	if (q->tail) {
		q->tail->next = node;
	}
	q->tail = node;
	if (!q->head) {
		q->head = q->tail;
	}
	pthread_cond_signal(&q->cond);
	pthread_mutex_unlock(&q->mutex);
	return 0;
}
void *msgq_pop(msgq_t *q)
{
	if (!q) {
		return NULL;
	}
	int err = pthread_mutex_lock(&q->mutex);
	if (err) {
		return NULL;
	}
	while (!q->head && !q->shutdown) {
		pthread_cond_wait(&q->cond, &q->mutex);
	}
	if (q->shutdown) {
		pthread_mutex_unlock(&q->mutex);
		return NULL;
	}

	msgq_node_t *node = q->head;
	void *ret = node->data;
	q->head = q->head->next;
	if (!q->head) {
		q->tail = NULL;
	}
	free(node);
	pthread_mutex_unlock(&q->mutex);
	return ret;
}

void *msgq_try_pop(msgq_t *q)
{
	if (!q) {
		return NULL;
	}
	int err = pthread_mutex_lock(&q->mutex);
	if (err) {
		return NULL;
	}

	if (!q->head || q->shutdown) {
		pthread_mutex_unlock(&q->mutex);
		return NULL;
	}

	msgq_node_t *node = q->head;
	q->head = node->next;
	if (!q->head) {
		q->tail = NULL;
	}
	pthread_mutex_unlock(&q->mutex);
	void *data = node->data;
	free(node);
	return data;
}

void msgq_shutdown(msgq_t *q)
{
	if (!q) {
		return;
	}
	int err = pthread_mutex_lock(&q->mutex);
	if (err) {
		return;
	}
	q->shutdown = true;
	pthread_cond_broadcast(&q->cond);
	pthread_mutex_unlock(&q->mutex);
}

void msgq_deinit(msgq_t *q)
{
	if (!q) {
		return;
	}
	int err = pthread_mutex_lock(&q->mutex);
	if (err) {
		return;
	}
	msgq_node_t *node = q->head;
	msgq_node_t *next_node = NULL;
	while (node) {
		next_node = node->next;
		free(node);
		node = next_node;
	}
	q->head = q->tail = NULL;
	pthread_mutex_unlock(&q->mutex);

	pthread_cond_destroy(&q->cond);
	pthread_mutex_destroy(&q->mutex);
}
