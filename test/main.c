#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include <string.h>
#include "queue.h"
#include <stdint.h>
// #include <semaphore.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif
#ifdef __APPLE__
dispatch_semaphore_t empty;
#else
sem_t empty;
#endif

queue_t my_queue;
void high_water_mark_evt(void *queue)
{
	(void) queue;
	// sem_wait(&empty);
	pthread_mutex_unlock(&((queue_t *)queue)->mutex); /* release the queue mutex */
#ifdef __APPLE__
	dispatch_semaphore_wait(empty, DISPATCH_TIME_FOREVER);
#else
	sem_wait(&empty);
#endif

	pthread_mutex_lock(&((queue_t *)queue)->mutex); /* lock the queue mutex */
}

void low_water_mark_evt(void *queue)
{
	(void) queue;
#ifdef __APPLE__
	// sem_post(&empty);
	dispatch_semaphore_signal(empty);
#else
	sem_post(&empty);
#endif

}

void *producer(void *vargp)
{
	//resolve the unused variable
	(void)vargp;
	int thread_id =  *(int *)vargp;
	printf("thread_id %x\n", thread_id);
	char *buffer = NULL;
	for(int i = 0; i < 20; i ++){
		buffer = malloc(sizeof(char) * 50);
    	sprintf(buffer, "task %d %d ", thread_id, i);
    	queue_send(&my_queue, (void *)buffer, thread_id);
    	// sleep(1);
	}
    return NULL;
}


void *consumer(void *vargp)
{
	//resolve the unused variable
	(void)vargp;
	int thread_id =  *(int *)vargp;
	printf("thread_id %x\n", thread_id);
	uint8_t *data;
	while(1){
		data = (uint8_t *)queue_get(&my_queue);
		if(data != NULL){
    		printf("task %d:received %s \n",thread_id, data);
    		sleep(1);
    		free(data);
	    } else {
	    	sleep(1);
	    	printf("queue empty\n");
	    }
	}

	return NULL;
}

void test_thread_safe(void)
{
    pthread_t producer_thread_id;
    pthread_t consumer_thread_id;
#ifdef __APPLE__
    empty = dispatch_semaphore_create(1);
#else
	sem_init(&empty, 0,0);
#endif
    my_queue.size = 10;
	queue_set_high_water_mark(&my_queue, 9);
	queue_set_low_water_mark(&my_queue, 1);
	my_queue.high_water_mark_clbk = &high_water_mark_evt;
	my_queue.low_water_mark_clbk = &low_water_mark_evt;
    queue_create(&my_queue);
    for (int i = 0; i < 2; ++i)
    {
		int *arg = malloc(sizeof(int));
		*arg = i; 
    	pthread_create(&producer_thread_id, NULL, producer, arg);
    }

    for (int i = 0; i < 2; ++i)
    {
    	int *arg = malloc(sizeof(int));
		*arg = i;
    	pthread_create(&consumer_thread_id, NULL, consumer, arg);
    }
    pthread_join(producer_thread_id, NULL);
    pthread_join(consumer_thread_id, NULL);

    queue_delete(&my_queue);
}

void test_priority(void)
{
  	my_queue.size = 10;
	my_queue.high_water_mark_clbk = NULL;
	my_queue.low_water_mark_clbk = NULL;
    queue_create(&my_queue);

	char *buffer = NULL;
	for(int i = 0; i < 9; i ++){
		buffer = malloc(sizeof(char));
    	*buffer = (char)i;
    	queue_send(&my_queue, (void *)buffer, i);
	}

	uint8_t *data;
	for(int i = 0; i < 9; i ++){
		data = (uint8_t *)queue_get(&my_queue);
		if(data != NULL){
    		printf("received %d \n", *data);
    		// sleep(1);
    		free(data);
	    } else {
	    	sleep(1);
	    	printf("queue empty\n");
	    }

	}


}

void test_full(void){
	queue_t *ret;
	my_queue.size = 10;
	my_queue.high_water_mark_clbk = NULL;
	my_queue.low_water_mark_clbk = NULL;
	queue_create(&my_queue);

	char *buffer = NULL;
	for(int i = 0; i < 12; i ++){
		buffer = malloc(sizeof(char));
    	*buffer = (char)i;
    	ret = queue_send(&my_queue, (void *)buffer, i);
    	if(ret == NULL){
    		// printf("queue full at i = %d\n", i);
    		if(i >= my_queue.size){
    			printf("test full passed \n");
    			return;
    		} else {
    			printf("test full failed \n");
    			return;
    		}
    	}
	}
}

void test_empty(void){
	my_queue.size = 10;
	my_queue.high_water_mark_clbk = NULL;
	my_queue.low_water_mark_clbk = NULL;
	queue_create(&my_queue);
	uint8_t *data;
	data = (uint8_t *)queue_get(&my_queue);
	if(data == NULL) {
		printf("test empty passed\n");
	} else {
		printf("test empty failed\n");
	}

}

int main()
{
	// test_priority();
	test_thread_safe();
	// test_empty();
	// test_full();
    exit(0);
}
