#include <stdio.h>
#include <pthread.h>

#define DEFAULT_QUEUE_SIZE 50
#define DEFAULT_THREAD_NUMBER 50

struct threadPoolStruct {
    int threadNum, taskNum;
    void **tasks;
    pthread_t *threads;
};

typedef threadPoolStruct threadPool;
