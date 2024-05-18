#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>
#include <omp.h>

//Parametrização do número de sensores e atuadores
#define NUM_SENSORS 4
#define NUM_ACTUATORS 4
#define BUFFER_SIZE 100
#define TRUE 1
#define FALSE 0


pthread_t sensorsThread[NUM_SENSORS];
pthread_t controlCenterThread, simulatedThreadPoolThread;
int sensorialData[NUM_SENSORS];
int sensorsId[NUM_SENSORS];
int sensorialDataIn, sensorialDataOut = 0;
int actuatorsValue[NUM_ACTUATORS] = {0};
int buffer[BUFFER_SIZE];
int buffer_count = 0;

//Declaração de semáforos e mutexes
sem_t sensorialDataBufferEmpty, sensorialDataBufferFull, activityBuffer;
pthread_mutex_t createSensorialDataMutex, writeOnScreenMutex, readSensorialDataMutex, bufferMutex;
pthread_mutex_t actuatorsMutex[NUM_ACTUATORS];

//Declaração de funções
void *createSensorialData(void *arg);
int readSensorialData();
void *controlCenter();
void modifyActuatorValue(int data, int actuator);
void writeOnScreen(int data, int actuator);

//Função para criar um novo dado sensorial
void *createSensorialData(void *arg) {
    int thread_num = *((int *) arg);
    while (TRUE) {
        sem_wait(&sensorialDataBufferEmpty);
        pthread_mutex_lock(&createSensorialDataMutex);
        srand(time(NULL));
        //Intervalo para conseguir um novo dado sensorial entre 1 a 5 segundos
        sensorialData[sensorialDataIn] = (rand() % 1000 + 1);
        sensorialDataIn = (sensorialDataIn + 1) % NUM_SENSORS;
        sleep((rand() % 5) + 1);
        pthread_mutex_unlock(&createSensorialDataMutex);
        sem_post(&sensorialDataBufferFull);
    }
}

int readSensorialData() {
    sem_wait(&sensorialDataBufferFull);
    int data = sensorialData[sensorialDataOut];
    sensorialDataOut = (sensorialDataOut + 1) % NUM_SENSORS;
    sem_post(&sensorialDataBufferEmpty);
    return data;
}

void *simulatedThreadPool() {
    #pragma omp parallel
        {
            while (TRUE) {
                sem_wait(&activityBuffer);
                pthread_mutex_lock(&bufferMutex);
                buffer_count--;
                int actuator = buffer[buffer_count];
                pthread_mutex_lock(&actuatorsMutex[actuator]);
                pthread_mutex_unlock(&bufferMutex);
                int actuatorNewValue = (rand() % 100) + 1;
                pid_t p = fork();
                int processStatus;
                int childProcessStatus;
                if (p < 0) {
                    printf("Erro no fork\n");
                    exit(1);
                } else if (p == 0) {
                    modifyActuatorValue(actuatorNewValue, actuator);
                } else {
                    if (rand() % 5 == 0) {
                        processStatus = 1;  
                    }
                    wait(&childProcessStatus);
                    if (WEXITSTATUS(childProcessStatus) == 1 || processStatus == 1) {
                        pthread_mutex_lock(&writeOnScreenMutex);
                        printf("Falha: <%d>\n", actuator+1);
                        sleep(1);
                        pthread_mutex_unlock(&writeOnScreenMutex);
                    } else {
                        writeOnScreen(actuatorNewValue, actuator);
                    }
                }
                processStatus = 0;
                pthread_mutex_unlock(&actuatorsMutex[actuator]);
            }
        }
}

void *controlCenter() {
    pthread_create(&simulatedThreadPoolThread, NULL, simulatedThreadPool, NULL);
    while (TRUE) {
        int data = readSensorialData();
        int actuator = data % NUM_ACTUATORS;
        // printf("actuator: %d\n", actuator);
        pthread_mutex_lock(&bufferMutex);
        buffer[buffer_count] = actuator;
        // printf("actuator in array: %d\n", buffer[buffer_count]);
        buffer_count++;
        // printf("buffer count: %d\n", buffer_count);
        pthread_mutex_unlock(&bufferMutex);
        sem_post(&activityBuffer);
    }
}

void modifyActuatorValue(int data, int actuator) {
    if (rand() % 5 == 0) {
        exit(1);
    } else {
        sleep((rand() % 2) + 2);
        actuatorsValue[actuator] = data;
        exit(0);
    }
}

void writeOnScreen(int data, int actuator) {
    pthread_mutex_lock(&writeOnScreenMutex);
    printf("Alterando: <%d> com valor <%d>!\n", actuator+1, data);
    sleep(1);
    pthread_mutex_unlock(&writeOnScreenMutex);
}

int main() {
    //Inicialização dos semáforos e mutexes
    sem_init(&sensorialDataBufferEmpty, 0, NUM_SENSORS); //Semáforo para produção
    sem_init(&sensorialDataBufferFull, 0, 0); //Semáforo para consumo
    sem_init(&activityBuffer, 0, 0); //Semáforo para simular threadpool
    pthread_mutex_init(&createSensorialDataMutex, NULL);
    pthread_mutex_init(&writeOnScreenMutex, NULL);
    pthread_mutex_init(&readSensorialDataMutex, NULL);
    pthread_mutex_init(&bufferMutex, NULL);
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        pthread_mutex_init(&actuatorsMutex[i], NULL);
    }

    for (int i = 0; i < NUM_SENSORS; i++) {
        sensorsId[i] = i;
        pthread_create(&sensorsThread[i], NULL, createSensorialData, &sensorsId[i]);
    }

    pthread_create(&controlCenterThread, NULL, controlCenter, NULL);

    for (int i = 0; i < NUM_SENSORS; i++) {
        pthread_join(sensorsThread[i], NULL);
    }
    
    //Destruição dos semáforos e mutexes
    sem_destroy(&sensorialDataBufferEmpty);
    sem_destroy(&sensorialDataBufferFull);
    sem_destroy(&activityBuffer);
    pthread_mutex_destroy(&createSensorialDataMutex);
    pthread_mutex_destroy(&writeOnScreenMutex);
    pthread_mutex_destroy(&readSensorialDataMutex);
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        pthread_mutex_destroy(&actuatorsMutex[i]);
    }
    return 0;
}
