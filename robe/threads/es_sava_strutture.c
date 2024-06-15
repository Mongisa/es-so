/* sava
Il programma prevede di ricevere da linea di comando un numero pari N, 1<N<51.
Si creino N threads, di cui la prima metà sono produttori e la seconda i consumatori.
Ogni produttore  ha un proprio id che va da 1 a N/2
Ogni consumatore ha un proprio id che va da 1 a N/2 

Ogni produttore chiede da tastiera una stringa, attraverso un prompt che deve 
specificare il proprio id: (N=10) output ->
PRODUTTORE (id=4) -> richiedo stringa: ciaoo
Ora può riempire il buffer condiviso 

Ogni consumatore legge dal buffer, stampando:
CONSUMATORE (id=10) -> prelevo stringa: ciaoo, dal produttore (id=4)

Si adotti un'implementazione della sincronizzazione tramite l'utilizzo di due 
condvar (libero,occupato) da parte dei produttori e consumatori, nonostante ne basti una.

Il thread principale è in attesa infinita, per cui il programma non deve terminare
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

typedef struct{
    int x;
    char str[100];
}Telementi;

typedef struct{
    Telementi e;
    bool completo;
    pthread_mutex_t mutex;
    pthread_cond_t vuoto;
    pthread_cond_t pieno;
}TBuffer;

typedef struct{
    int id;
    TBuffer* buffer;
}TParam;

void* produttore(void*);
void* consumatore(void*);

int main(int argc, char* argv[]){
    if(argc != 2) {
        printf("Fornire N!\n");
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    if(n<=1 || n>=51 || n%2 != 0) {
        printf("Fornire 1<N<51");
        return 1;
    }
    
    TBuffer b;
    
    b.e.x=0;
    b.completo=false;
    pthread_mutex_init(&b.mutex,NULL);
    pthread_cond_init(&b.vuoto,NULL);
    pthread_cond_init(&b.pieno,NULL);


    TParam p_prod[n/2];
    TParam p_cons[n/2];
    pthread_t tid_prod[n/2];
    pthread_t tid_cons[n/2];
    
    for(int i=0;i<n/2;i++) {
        p_prod[i].buffer = &b;
        p_prod[i].id = i+1;
        
        if(pthread_create(&tid_prod[i], NULL, produttore, &p_prod[i]) != 0) {
            printf("Pthread creation err\n");
            return 2;
        }
        
        p_cons[i].buffer = &b;
        p_cons[i].id = i+1;
        
        if(pthread_create(&tid_cons[i], NULL, consumatore, &p_cons[i]) != 0) {
            printf("Pthread creation err\n");
            return 2;
        }
    }
    
    for(int i=0;i<n/2;i++) {
        pthread_join(tid_prod[i], NULL);
        pthread_join(tid_cons[i], NULL);
    }
    
    return 0;
}

void* produttore(void* arg){
    TParam* p = (TParam*) arg;
    TBuffer* b = p->buffer;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->completo == true)
            pthread_cond_wait(&b->vuoto,&b->mutex);
            
        printf("Inserisci stringa (id=%d): \n",p->id);
        scanf("%s",b->e.str);
        
        b->e.x=p->id;
        b->completo=true;
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_broadcast(&b->pieno);
    }
}

void* consumatore(void* arg){
    TParam* p = (TParam*) arg;
    TBuffer* b = p->buffer;
    
    while(true) {
        pthread_mutex_lock(&b->mutex);
        
        while(b->completo == false)
            pthread_cond_wait(&b->pieno,&b->mutex);
            
        printf("prelevo stringa %s dal produttore (id=%d)\n",b->e.str,b->e.x);
        
        b->completo=false;
        
        pthread_mutex_unlock(&b->mutex);
        pthread_cond_broadcast(&b->vuoto);
    }
}
