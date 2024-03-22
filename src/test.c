#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#define ll long long

#define MAX_THREAD_COUNT 3
pthread_t *thread_arr;
pthread_mutex_t read_mutex;
int cnt = 0, total_input_num;

void *func(void *null){
    // a loop to wait for input to handle
    while(1){
        int opt, val;
        // try to read from stdin
        pthread_mutex_lock(&read_mutex);
        if (cnt == total_input_num){
            pthread_mutex_unlock(&read_mutex);
            break;
        }
        cnt ++;
        // scanf("%d %d", &opt, &val);
        scanf("%d", &opt);
        printf("Thread %ld captures the %d-th input!\n", pthread_self(), cnt);
        pthread_mutex_unlock(&read_mutex);

        // simulate the payload
        sleep(5);
    }
}

int main(){
    // input the number of instructions
    scanf("%d", &total_input_num);

    // thread pool
    thread_arr = (pthread_t *) malloc(sizeof(pthread_t) * MAX_THREAD_COUNT);
    // init semaphore and mutex
    pthread_mutex_init(&read_mutex, NULL);
    
    // feed all the thread in thread_pool with an initial routine
    for (int i = 0; i < MAX_THREAD_COUNT; i++)
        pthread_create(thread_arr + i, NULL, func, NULL);

    // clean-up
    for (int i = 0; i < MAX_THREAD_COUNT; i++)
        while (pthread_join(thread_arr[i], NULL));
    pthread_mutex_destroy(&read_mutex);


    return 0;
}