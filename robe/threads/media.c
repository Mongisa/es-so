#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#define TIME 60
#define BUFF_SIZE 5
#define SLEEP 2
#define MAX 100

typedef struct {
    int vet[BUFF_SIZE];
    int count;
    int start;
    int end;
    bool ready;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

void* prod(void* arg);
void* cons(void* arg);

int main() {
    TBuffer buffer;

    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.cond, NULL);
    buffer.count=0;
    buffer.start=0;
    buffer.end=0;
    buffer.ready=false;
    srand(time(NULL));
    
    pthread_t tid[2];
    
    if(pthread_create(&tid[0], NULL, prod, &buffer) != 0) {
        printf("Thread creation err\n");
        return 1;
    }
    
    if(pthread_create(&tid[1], NULL, cons, &buffer) != 0) {
        printf("Thread creation err\n");
        return 1;
    }
    
    sleep(TIME);
    return 0;
    
}

void* prod(void* arg) {
    TBuffer* b = (TBuffer *) arg;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->ready==true)
            pthread_cond_wait(&b->cond,&b->mutex);
        
        int n = rand() % (MAX + 1);
        
        b->vet[b->start] = n;
        b->start = (b->start+1) % BUFF_SIZE;
        b->count++;
        b->ready=true;
        
        printf("PRODUTTORE: NUM=%d\n", n);
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_signal(&b->cond);
        
        sleep(SLEEP);
        
    }
}

void* cons(void* arg) {
    TBuffer* b = (TBuffer *) arg;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->ready==false)
            pthread_cond_wait(&b->cond,&b->mutex);
            
        float media = 0;
            
        for(int i=0; i<b->count; i++) {
            media += b->vet[i];
        }
        
        media = media/b->count;
        
        printf("CONSUMATORE: MEDIA=%.2f\n",media);
        
        b->ready=false;
        
        if(b->count>5) {
            b->count--;
        }
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_signal(&b->cond);
    }
}
