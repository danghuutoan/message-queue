#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include <string.h>
#include "queue.h"
// #include <semaphore.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

char *test1 ="hello1";
char *test2 ="hello2";

dispatch_semaphore_t empty;
queue_t my_queue;
void high_water_mark_evt(void *queue)
{
	// (void) queue;
	// sem_wait(&empty);
	// long ret = 1;
	// printf("high_water_mark_evt\n");
	pthread_mutex_unlock(&((queue_t *)queue)->mutex); /* release the queue mutex */
	dispatch_semaphore_wait(empty, DISPATCH_TIME_FOREVER);
	pthread_mutex_lock(&((queue_t *)queue)->mutex); /* lock the queue mutex */
	// printf("high_water_mark_evt return\n");
}

void low_water_mark_evt(void *queue)
{
	(void) queue;
	// sem_post(&empty);
	// printf("low_water_mark_evt length %d\n", ((queue_t *)queue)->length);
	dispatch_semaphore_signal(empty);
}

void *producer(void *vargp)
{
	//resolve the unused variable
	(void)vargp;
	int thread_id =  *(int *)vargp;
	printf("thread_id %x\n", thread_id);
	char *buffer = NULL;
	for(int i = 0; i < 1000; i ++){
		buffer = malloc(sizeof(char) * 50);
    	sprintf(buffer, "task %d %d ", thread_id, i);
    	// printf("sending %s \r\n", buffer);
    	queue_send(&my_queue, (void *)buffer, 0);
    	// sleep(1);
	}
    return NULL;
}


void *consumer(void *vargp)
{
	//resolve the unused variable
	(void)vargp;
	uint8_t *data;
	while(1){
		data = (uint8_t *)queue_get(&my_queue);
		if(data != NULL){
    		printf("received %s \r\n", data);
    		// sleep(1);
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
    my_queue.size = 10;
   	my_queue.high_water_mark = 9;
	my_queue.low_water_mark = 1;
	my_queue.high_water_mark_clbk = &high_water_mark_evt;
	my_queue.low_water_mark_clbk = &low_water_mark_evt;
    queue_create(&my_queue);
    for (int i = 0; i < 6; ++i)
    {
		int *arg = malloc(sizeof(int));
		*arg = i; 
    	pthread_create(&producer_thread_id, NULL, producer, arg);
    }
    pthread_create(&consumer_thread_id, NULL, consumer, NULL);
    pthread_join(producer_thread_id, NULL);
    pthread_join(consumer_thread_id, NULL);
}

void test_high_water_mark(void)
{
    pthread_t producer_thread_id;
    pthread_t consumer_thread_id;
    // sem_init(&empty, 0, 0);
    empty = dispatch_semaphore_create(1);
    my_queue.size = 10;
   	my_queue.high_water_mark = 9;
	my_queue.low_water_mark = 1;
	my_queue.high_water_mark_clbk = &high_water_mark_evt;
	my_queue.low_water_mark_clbk = &low_water_mark_evt;
    queue_create(&my_queue);
    for (int i = 0; i < 10; ++i)
    {
		int *arg = malloc(sizeof(int));
		*arg = i; 
    	pthread_create(&producer_thread_id, NULL, producer, arg);
    }
    pthread_create(&consumer_thread_id, NULL, consumer, NULL);
    pthread_join(producer_thread_id, NULL);
    pthread_join(consumer_thread_id, NULL);

}

int main()
{
	// test_thread_safe();
	test_high_water_mark();
    exit(0);
}