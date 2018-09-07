#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include <string.h>
#include "queue.h"

queue_t *queue_create(queue_t *queue)
{
	queue->tail = NULL;
	queue->head = NULL;
	queue->length = 0;
	pthread_mutex_init(&queue->mutex, NULL);
	return queue;
}

queue_t *queue_send(queue_t *queue, void *data, int priority)
{
	pthread_mutex_lock(&queue->mutex); /* lock the queue mutex */
	node_t *new_node = malloc(sizeof(node_t));

	if(queue->length >= queue->high_water_mark)
	{
		queue->high_water_mark_clbk(queue);
	}

	if(new_node){
		new_node->data = data;
		new_node->next = queue->head;
		new_node->priority = priority;
		queue->head = new_node;
		queue->length++;
	} else {
		return NULL;
	}
	pthread_mutex_unlock(&queue->mutex); /* release the queue mutex */
	return queue;
}


void *queue_get(queue_t *queue)
{
	int min = QUEUE_MAX_PRIORITIES;
	void *ret_val = NULL;
	pthread_mutex_lock(&queue->mutex); /* lock the queue mutex */
	node_t *return_element = NULL;
	node_t *prev_element = NULL;
	node_t *element = queue->head;
	node_t *prev_min;


	while(element != NULL){
		// find the highest priority element
		if(min > element->priority){
			min = element->priority;
			return_element = element;
			ret_val = (void *)element->data;
			prev_min = prev_element;
		}

		prev_element = element;
		element = element->next;
	}

	//remove node out of linked list
	if(return_element != NULL){
		if(return_element == queue->head){
			queue->head = return_element->next;
		}
		else {
			prev_min->next = return_element->next;
		};
		queue->length--;
		//free the node memory
		free(return_element);
	}

	if(queue->length <= queue->low_water_mark)
	{
		queue->low_water_mark_clbk(queue);
	}
	pthread_mutex_unlock(&queue->mutex); /* release the queue mutex */
	return ret_val;
}