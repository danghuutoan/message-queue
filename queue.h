#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include <string.h>
#include <stdint.h>

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
	uint8_t high_water_mark;
	uint8_t low_water_mark;
	void (*high_water_mark_clbk)(void *queue);
	void (*low_water_mark_clbk)(void *queue);
} queue_t;

queue_t *queue_create(queue_t *queue);
void queue_delete(queue_t *queue);
queue_t *queue_send(queue_t *queue, void *data, int priority);
void *queue_get(queue_t *queue);

void queue_set_high_water_mark(queue_t *queue, uint8_t value);
void queue_set_low_water_mark(queue_t *queue, uint8_t value);
