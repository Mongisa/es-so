/*
Scriverere un programma in C sulla sincronizzazione dei thread. Il programma è
composto da 1 thread produttore e 3 consumatori che lavorano in maniera coordinata.

Thread produttore:
    1 Chiede all'utente il numero di elementi
    2 Alloca lo spazio al buffer per ospitare gli n elementi
    3 Chiede all'utente di immettere gli n elementi
    4 Risveglia i thread consumatori
    5 si riparte dal passo 1.

Thread consumatori:
    1 FASE DI CALCOLO: il primo calcola la somma degli n elementi [calc_sum],
    il secondo li ordina sfruttando la funzione bubble_sort() [sort],
    il terzo calcola la media [average]
    2 FASE DI STAMPA: ciascun thread stampa il suo risultato
    3 FASE DI DEALLOCAZIONE: l'ULTIMO dei thread ad accedere alla struttura
    provvede a deallocarla.
    4 si riparte dal passo 1.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct {
    int* vet;
    int size;
    bool is_full;
    int thread_count[3];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

void bubble_sort(int*,int);
void* prod(void* arg);
void* calc_sum(void* arg);
void* sort(void* arg);
void* average(void* arg);

int main() {
    TBuffer buffer;
    
    buffer.is_full=false;
    pthread_mutex_init(&buffer.mutex,NULL);
    pthread_cond_init(&buffer.cond,NULL);
    buffer.size=0;
    
    pthread_t tid[4];
    
    if(pthread_create(&tid[0],NULL,prod,&buffer) != 0) {
        printf("Errore creazione thread\n");
        return 2;
    }
    
    if(pthread_create(&tid[1],NULL,calc_sum,&buffer) != 0) {
        printf("Errore creazione thread\n");
        return 2;
    }
    
    if(pthread_create(&tid[2],NULL,sort,&buffer) != 0) {
        printf("Errore creazione thread\n");
        return 2;
    }
    
    if(pthread_create(&tid[3],NULL,average,&buffer) != 0) {
        printf("Errore creazione thread\n");
        return 2;
    }
    
    for(int i=0;i<4;i++)
        pthread_join(tid[i],NULL);
    
    return 0;
}

void* prod(void* arg) {
    TBuffer *b = (TBuffer *) arg;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->is_full==true)
            pthread_cond_wait(&b->cond,&b->mutex);
            
        do {
            printf("Inserisci numero elementi: ");
            scanf("%d",&b->size);
        } while(b->size < 1);
        
        b->vet = malloc(b->size*sizeof(int));
        
        for(int i=0;i<b->size;i++) {
            printf("Inserisci [%d] elemento: ",i);
            scanf("%d", &b->vet[i]);
        }
        
        for(int i=0;i<3;i++)
            b->thread_count[i]=0;
        
        b->is_full=true;
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_broadcast(&b->cond);
    }
}

void* calc_sum(void* arg) {
    TBuffer *b = (TBuffer *) arg;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->is_full==false || b->thread_count[0]==1)
            pthread_cond_wait(&b->cond,&b->mutex);
            
        int sum=0;
        
        for(int i=0;i<b->size;i++)
            sum += b->vet[i];
            
        printf("CALC_SUM: SOMMA=%d\n",sum);
            
        b->thread_count[0]=1;
        
        if(b->thread_count[0] && b->thread_count[1] && b->thread_count[2]) {
            free(b->vet);
            b->is_full=false;
        }
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_broadcast(&b->cond);
    }
}

void* sort(void* arg) {
    TBuffer *b = (TBuffer *) arg;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->is_full==false || b->thread_count[1]==1)
            pthread_cond_wait(&b->cond,&b->mutex);
            
        bubble_sort(b->vet,b->size);
        
        printf("SORT: VETTORE_ORDINATO:");
        for(int i=0;i<b->size;i++)
            printf(" %d",b->vet[i]);
        printf("\n");
            
        b->thread_count[1]=1;
        
        if(b->thread_count[0] && b->thread_count[1] && b->thread_count[2]) {
            free(b->vet);
            b->is_full=false;
        }
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_broadcast(&b->cond);
    }
}

void* average(void* arg) {
    TBuffer *b = (TBuffer *) arg;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->is_full==false || b->thread_count[2]==1)
            pthread_cond_wait(&b->cond,&b->mutex);
            
        float av=0;
        
        for(int i=0;i<b->size;i++)
            av += b->vet[i];
        
        av = av/b->size;
        
        printf("AVERAGE: MEDIA=%.3f\n",av);
            
        b->thread_count[2]=1;
        
        if(b->thread_count[0] && b->thread_count[1] && b->thread_count[2]) {
            free(b->vet);
            b->is_full=false;
        }
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_broadcast(&b->cond);
    }
}

void bubble_sort(int a[], int n) {
    int i,k;
    bool modified;

    modified = true;
    for(k=0; k<n-1 && modified; k++) {
        modified = false;
        for(i=0; i<n-k-1; i++) {
            if(a[i]>a[i+1]) {
                int tmp = a[i];
                a[i]=a[i+1];
                a[i+1]=tmp;
                modified = true;
            }
        }
    }
}
