/*Per ogni numero naturale n si definisce n-mo numero armonico alternato la somma 
h_n=1 - 1/2 + 1/3-1/4 +... (-1)^n+1 1/n
la funzione fornita calcola il numero armonico alternato corrispondente
al numero naturale n.

Il candidato completi il programma fornito, completando
il main ed implementando le funzioni producer_thread ed consumer_thread.
il main crea due thread, eseguono le due funzioni citate.
I due  thread condividono un buffer, definito attraverso la struttura
TBuffer che può contenere un numero oppure essere vuoto. 

Il thread producer ripete le seguenti operazioni:
1 aspetta che il buffer sia vuoto
2 legge un numero naturale da tastiera con scanf inserendolo nel buffer
3 rende il buffer non vuoto
4 ricomincia da 1 

Il thread consumer ripete le seguenti operazioni:
1 aspetta che il buffer sia non vuoto
2 calcola e stampa il numero armonico del numero nel buffer
3 rende il buffer vuoto
4 ricomincia da 1

l'inserimento di un numero minore o uguale a 0 termina il programma

esempio:
l'utente inserisce i numeri: 7 3 1 -1
Producer: inserisci un numero: 7
Consumer: numero alternato di 7 = 0.759524
Producer: inserisci un numero: 3
Consumer:
Producer:inserisci un numero: 1
Consumer:
Producer:inserisci un numero: -1
Consumer:
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    int number;
    bool vuoto;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

void *producer_thread(void *);
void *consumer_thread(void *);

double HarmonicNumber(int n) {
    double harmonic_number = 0;
    for(int i=0;i<n;i++)
        if(i%2==0)
            harmonic_number += 1.0/(1+i);
        else
            harmonic_number -= 1.0/(i+1);
    return harmonic_number;
}

int main(int argc, char* argv[]) {
    pthread_t tid[2];
    
    TBuffer b;
    
    b.vuoto=true;
    pthread_mutex_init(&b.mutex,NULL);
    pthread_cond_init(&b.cond,NULL);
    
    if(pthread_create(&tid[0],NULL,producer_thread,&b) != 0) {
        printf("pthread creation err\n");
        return 2;
    }
    
    if(pthread_create(&tid[1],NULL,consumer_thread,&b) != 0) {
        printf("pthread creation err\n");
        return 2;
    }
    
    pthread_join(tid[0],NULL);
    
    return 0;
}

void *producer_thread(void *param) {
    TBuffer *b = (TBuffer *) param;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->vuoto==false)
            pthread_cond_wait(&b->cond,&b->mutex);
        
        printf("Inserisci numero: ");
        scanf("%d",&b->number);
        
        if(b->number <= 0) {
            printf("Terminazione\n");
            return NULL;
        }
        
        b->vuoto=false;
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_signal(&b->cond);
    }
    
    return NULL;
}

void *consumer_thread(void *param) {
    TBuffer *b = (TBuffer *) param;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->vuoto==true)
            pthread_cond_wait(&b->cond,&b->mutex);

        printf("Numero armonico: %.6f\n",HarmonicNumber(b->number));
        
        b->vuoto=true;
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_signal(&b->cond);
    }
    
    return NULL;
}
