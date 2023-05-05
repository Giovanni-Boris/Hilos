/*
 *  Descripcion: 
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TOTAL_GAMES 5

//variables globales
char* playerNames[2] = {"", ""};
int gameCount = 0;
int gameResult[2] = {0, 0};
int gameIndividualCount[2] = {0, 0};
int gameIndividualResult[2] = {0, 0};
pthread_mutex_t mutex;

typedef struct parameters{
    int id;
    const char* player;
}parameter;

void* play(void *arg){
    parameter* param = (parameter*) arg;

    while(gameIndividualCount[param->id] <= gameIndividualCount[1 - param->id] && (gameIndividualCount[0] + gameIndividualCount[1] < 2 * TOTAL_GAMES)){
        //atómico
        pthread_mutex_lock(&mutex);
        //generación del tiro
        int throw = (rand() % 6) + 1;
        gameIndividualCount[param->id]++;
        gameIndividualResult[param->id] = throw;
        printf("Game %d -> [%s] obtained %d\n", gameCount + 1, param->player, throw);

        //consulta de definición del juego N-esimo
        if(gameIndividualCount[0] == gameIndividualCount[1]){
            if(gameIndividualResult[0] != gameIndividualResult[1]){
                int idWinner = (gameIndividualResult[0] > gameIndividualResult[1])?0:1;
                gameResult[idWinner]++;
                printf("\tWinner: %s\n", playerNames[idWinner]);
            }else{
                printf("\tNo winner\n");
            }
            gameCount++;
            //sleep(1);
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

int main(){
    //inicializando rand
    srand(time(NULL));

    //inicializando mutex
    pthread_mutex_init(&mutex, NULL);

    //parametros para cada hilo
    parameter p1, p2;
    p1.id = 0; p1.player = playerNames[0] = "player 1";
    p2.id = 1; p2.player = playerNames[1] = "player 2";

    //creacion de hilos y juego
    pthread_t h1, h2;
    while(gameCount < TOTAL_GAMES){
        pthread_create(&h1, NULL, play, (void*) &p1);
        pthread_create(&h2, NULL, play, (void*) &p2);
        pthread_join(h1, NULL);
        pthread_join(h2, NULL);
    }
    printf("EndGame\n");

    //imprimiendo mas informacion
    printf("\nResults\n----------------------------------------\n");
    printf("Game count: %d\n", gameCount);
    if(gameResult[0] == gameResult[1]) {
        printf("No winner! ");
    }else{
        printf("Absolute winner! ");
        printf("%s [%d vs %d]\n", playerNames[(gameResult[0] > gameResult[1]?0:1)], gameResult[0], gameResult[1]);
    }
}
