#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>

//semafori
sem_t semaforoAgente;
sem_t semaforoTabacco;
sem_t semaforoCarta;
sem_t semaforoFiammiferi;
sem_t tabacco;
sem_t fiammiferi;
sem_t carta;
sem_t semaforoMutex;

//condizioni ingredienti
bool tabaccoLibero = false;
bool cartaLibera = false;
bool fiammiferiLiberi = false;

//puntatori 
void *agenteA(void *);
void *agenteB(void *);
void *agenteC(void *);
void *pusherA(void *);
void *pusherB(void *);
void *pusherC(void *);
void *fumatore1(void *);
void *fumatore2(void *);
void *fumatore3(void *);

pthread_mutex_t stampa; //variabile sezioni critiche

int main( int argc, char *argv[] )  {
	
   pthread_t a1, a2, a3, p1, p2, p3, s1, s2, s3;
	
	printf("\n*-------------------------------------------------*\n");
	printf("\n|    Cigarette Smokers Catoggio Falanga Unibas    |\n");
	printf("\n*-------------------------------------------------*\n\n");

	sem_init(&semaforoAgente, 0, 1);
	sem_init(&semaforoTabacco, 0, 0);
	sem_init(&semaforoCarta, 0, 0);
	sem_init(&semaforoFiammiferi, 0, 0);
	sem_init(&tabacco, 0, 0);
	sem_init(&carta, 0, 0);
	sem_init(&fiammiferi, 0, 0);	
	sem_init(&semaforoMutex, 0, 1);
    pthread_mutex_init(&stampa, NULL);

    pthread_create(&a1, NULL, agenteA, NULL);
    pthread_create(&a2, NULL, agenteB, NULL);
    pthread_create(&a3, NULL, agenteC, NULL);
    pthread_create(&s1, NULL, fumatore1, NULL);
    pthread_create(&s2, NULL, fumatore2, NULL);
    pthread_create(&s3, NULL, fumatore3, NULL);
    pthread_create(&p1, NULL, pusherA, NULL);
    pthread_create(&p2, NULL, pusherB, NULL);
    pthread_create(&p3, NULL, pusherC, NULL);

	 while(true){}

}

//combinazioni ingredienti che può lasciare l'agente sul tavolo
void *agenteA(void *a){
	while(true){
		sem_wait(&semaforoAgente);
		sem_post(&semaforoTabacco);
		sem_post(&semaforoCarta);
	}
}

void *agenteB(void *b){
	while(true){
		sem_wait(&semaforoAgente);
		sem_post(&semaforoTabacco);
		sem_post(&semaforoFiammiferi);
	}
}

void *agenteC(void *c){
	while(true){
		sem_wait(&semaforoAgente);
		sem_post(&semaforoCarta);
		sem_post(&semaforoFiammiferi);
	}
}

void *fumatore1(void *a){
	while(true){
		pthread_mutex_lock(&stampa);
		printf("Fumatore 1 ha bisogno del tabacco\n");
		pthread_mutex_unlock(&stampa);
		
		sem_wait(&tabacco);

		pthread_mutex_lock(&stampa);
		printf("Fumatore 1 prende il tabacco, fuma\n");
		pthread_mutex_unlock(&stampa);
		
		sem_post(&semaforoAgente);

		pthread_mutex_lock(&stampa);
		printf("Fumatore 1 lascia il tabacco\n" );
		pthread_mutex_unlock(&stampa);
		sleep(4);
	}
}

void *fumatore2(void *b){
	while(true){
		pthread_mutex_lock(&stampa);
		printf("Fumatore 2 ha bisogno della carta\n");
		pthread_mutex_unlock(&stampa);

		sem_wait(&carta);

		pthread_mutex_lock(&stampa);
		printf("Fumatore 2 prende la carta, fuma\n");
		pthread_mutex_unlock(&stampa);

		sem_post(&semaforoAgente);
		pthread_mutex_lock(&stampa);
		printf("Fumatore 2 lascia la carta\n" );
		pthread_mutex_unlock(&stampa);
		sleep(4);
	}
}

void *fumatore3(void *c){
	while(true){
		pthread_mutex_lock(&stampa);
		printf("Fumatore 3 ha bisogno dei fiammiferi\n");
		pthread_mutex_unlock(&stampa);

		sem_wait(&fiammiferi);
		pthread_mutex_lock(&stampa);
		printf("Fumatore 3 prende i fiammiferi, fuma\n");
		pthread_mutex_unlock(&stampa);
		sem_post(&semaforoAgente);

		pthread_mutex_lock(&stampa);
		printf("Fumatore 3 lascia l'accendino'\n" );
		pthread_mutex_unlock(&stampa);

		sleep(4);
	}
}

void *pusherA(void *a){
	while(true){
			sem_wait(&semaforoTabacco);
			pthread_mutex_lock(&stampa);
			printf("Il tabacco e' sul tavolo.\n");
			pthread_mutex_unlock(&stampa);

			sem_wait(&semaforoMutex);
				if(cartaLibera){
					cartaLibera = false;
					sem_post(&carta);
				}else if(fiammiferiLiberi){
					fiammiferiLiberi = false;
					sem_post(&fiammiferi);
				}else{
					tabaccoLibero = true;
				}
			sem_post(&semaforoMutex);
	}
}


void *pusherB(void *b){
	while(true){
		sem_wait(&semaforoFiammiferi);
		pthread_mutex_lock(&stampa);
		printf("I fiammiferi sono sul tavolo.\n");
		pthread_mutex_unlock(&stampa);

		sem_wait(&semaforoMutex);
			if(cartaLibera){
				cartaLibera = false;
				sem_post(&fiammiferi);
			}else if(tabaccoLibero){
				tabaccoLibero = false;
				sem_post(&tabacco);
			}else{
				fiammiferiLiberi = true;
			}
		sem_post(&semaforoMutex);
	}
}

void *pusherC(void *c){
	while(true){
		sem_wait(&semaforoCarta);
		pthread_mutex_lock(&stampa);
		printf("La carta e' sul tavolo'.\n");
		pthread_mutex_unlock(&stampa);

		sem_wait(&semaforoMutex);
			if(tabaccoLibero){
				tabaccoLibero = false;
				sem_post(&tabacco);
			}else if(fiammiferiLiberi){
				fiammiferiLiberi = false;
				sem_post(&fiammiferi);
			}else{
				cartaLibera = true;
			}
		sem_post(&semaforoMutex);
	}
}
