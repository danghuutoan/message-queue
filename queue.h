#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include <string.h>


#define QUEUE_MAX_PRIORITIES 0xFF
typedef struct node
{
	int priority;
	void *data;
	struct node *next;
} node_t;

typedef struct queue
{
	node_t *tail;
	node_t *head;
	uint8_t size;
	uint8_t length;
	pthread_mutex_t mutex;
} queue_t;

queue_t *queue_create(uint8_t size);
queue_t *queue_send(queue_t *queue, void *data, int priority);
void *queue_get(queue_t *queue);