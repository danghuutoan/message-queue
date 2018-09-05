#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include <string.h>
#include "queue.h"

char *test1 ="hello1";
char *test2 ="hello2";

queue_t *my_queue = NULL;
// uint8_t data = 0;
void *producer(void *vargp)
{
	//resolve the unused variable
	(void)vargp;
	char *buffer = NULL;
	// static uint8_t count = 0;
	// char *buffer = (char *)malloc(sizeof(char) * 100);
	for(int i = 0; i < 100; i ++){
		buffer = malloc(sizeof(char) * 50);
    	sprintf(buffer, "task 0 %d ", i);
    	queue_send(my_queue, (void *)buffer, 0);
    	// printf("send %s\n", buffer);
    	sleep(1);
	}
    	// count++;
    

    return NULL;
}

void *producer1(void *vargp)
{
	//resolve the unused variable
	(void)vargp;
	char *buffer = NULL;
	for(int i = 0; i < 100; i ++){
		buffer = malloc(sizeof(char) * 50);
    	sprintf(buffer, "task 1 %d ", i);
    	queue_send(my_queue, (void *)buffer, 1);
    	// printf("send %s\n", buffer);
    	sleep(1);
	}
    return NULL;
}

void *producer2(void *vargp)
{
	//resolve the unused variable
	(void)vargp;
	// static uint8_t count = 0;
	char *buffer = NULL;
	for(int i = 0; i < 100; i ++){
		buffer = malloc(sizeof(char) * 50);
    	sprintf(buffer, "task 2 %d ", i);
    	queue_send(my_queue, (void *)buffer, 2);
    	// printf("send %s\n", buffer);
    	sleep(1);
	}

    return NULL;
}


void *consumer(void *vargp)
{
	//resolve the unused variable
	(void)vargp;
	uint8_t *data;
	while(1){
		data = (uint8_t *)queue_get(my_queue);
		if(data != NULL){
    		printf("received %s \r\n", data);
    		free(data);
	    } else {
	    	sleep(1);
	    }
	}

	return NULL;
}
  
int main()
{
    pthread_t producer_thread_id;
    pthread_t producer1_thread_id;
    pthread_t producer2_thread_id;
    pthread_t consumer_thread_id;
    my_queue = queue_create(10);
    printf("Before Thread\n");
    pthread_create(&producer_thread_id, NULL, producer, test1);
    pthread_create(&producer1_thread_id, NULL, producer1, test2);
    pthread_create(&producer2_thread_id, NULL, producer2, test2);
    pthread_create(&consumer_thread_id, NULL, consumer, NULL);
    pthread_join(producer_thread_id, NULL);
    pthread_join(producer1_thread_id, NULL);
    pthread_join(producer2_thread_id, NULL);
    pthread_join(consumer_thread_id, NULL);
    printf("After Thread\n");
    exit(0);
}