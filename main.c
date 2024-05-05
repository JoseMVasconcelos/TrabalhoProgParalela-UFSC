#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//Parametrização do número de sensores e atuadores
#define NUM_SENSORS 4
#define NUM_ACTUATORS 4

pthread_t sensorsThread[NUM_SENSORS];
int sensorialData[NUM_SENSORS];
int sensorsId[NUM_SENSORS];
//Como 
int actuatorsKey[NUM_ACTUATORS];
int actuatorsValue[NUM_ACTUATORS] = {0};
//Declaração de semáforos e mutexes
sem_t sensorialDataBufferEmpty, sensorialDataBufferFull;
pthread_mutex_t createSensorialDataMutex, modifyActuatorValueMutex;



//Função para criar um novo dado sensorial
void *createSensorialData(void *arg) {
    int thread_num = *((int *) arg);
    while(TRUE) {
        sem_wait(&sensorialDataBufferEmpty);
        srand(time(NULL));
        //Intervalo para conseguir um novo dado sensorial entre 1 a 5 segundos
        sleep((rand() % 5) + 1);
        printf("%d\n", (rand()%1000 + 1));
        sem_post(&sensorialDataBufferFull);
    }
}

// void readSensorialData() {
    // pass;
// }

void controlCenter() {
    sem_wait(&sensorialDataBufferFull);
    // int sensorialData = readSensorialData();
    // new thread 
    sem_post(&sensorialDataBufferEmpty);
}

void modifyActuatorValue() {
    sleep((rand() % 2) + 2);

    pthread_mutex_lock(&modifyActuatorValue);
    printf("Alterando: <ATUADOR> com valor <NIVEL DE ATIVIDADE>!\n");
    sleep(1);
    pthread_mutex_unlock(&modifyActuatorValue);

}

int main() {
    //Inicialização dos semáforos e mutexes
    sem_init(&sensorialDataBufferEmpty, 0, NUM_SENSORS); //Semáforo para produção
    sem_init(&sensorialDataBufferFull, 0, 0); //Semáforo para consumo
    pthread_mutex_init(&createSensorialDataMutex, NULL);
    pthread_mutex_init(&modifyActuatorValueMutex, NULL);


    for(int i = 0; i < NUM_SENSORS; i++) {
        sensorsId[i] = i;
        sleep(1);
        pthread_create(&sensorsThread[i], NULL, createSensorialData, &sensorsId[i]);
    }

    for(int i = 0; i < NUM_ACTUATORS; i++) {
        printf("Key: %d\n", actuatorsValue[i]);
    }

    for(int i = 0; i < NUM_SENSORS; i++) {
        pthread_join(sensorsThread[i], NULL);
    }
    //Destruição dos semáforos e mutexes
    sem_destroy(&sensorialDataBufferEmpty);
    sem_destroy(&sensorialDataBufferFull);
    pthread_mutex_destroy(&createSensorialDataMutex);
    pthread_mutex_destroy(&modifyActuatorValueMutex);
    return 0;
}