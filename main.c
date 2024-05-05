#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//Parametrização do número de sensores e atuadores
#define NUM_SENSORS 4
#define NUM_ACTUATORS 4


int sensorialData[NUM_SENSORS];
//Os atuadores são acessados via dicionário, como no C não existe essa estrutura
//implementei um dicionário simples.
typedef struct {
    int key;
    int value;
} dict;
dict actuatorsDict[NUM_ACTUATORS];

//função para adicionar um novo par chave-valor ao dicionário
void addToDict(int key, int value, int index) {
    actuatorsDict[index].key = key;
    actuatorsDict[index].value = value;
}
//função que inicia os atuadores com chave-valor de -1, 0 (Iniciando os atuadores com valor 0)
void startDict() {
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        addToDict(-1, 0, i);
    }
}

//Função para criar um novo dado sensorial
void createSensorialData() {
    sem_wait(&sensorialDataBufferEmpty);
    //Intervalo para conseguir um novo dado sensorial entre 1 a 5 segundos
    sleep((rand() % 5) + 1);
    pthread_mutex_lock(&createSensorialDataMutex);
    sensorialData append
    pthread_mutex_unlock(&createSensorialDataMutex);
    sem_post(&sensorialDataBufferFull);
}

void readSensorialData() {
    // pass;
}

void controlCenter() {
    sem_wait(&sensorialDataBufferFull);
    // int sensorialData = readSensorialData();
    // new thread 
    sem_post(&sensorialDataBufferEmpty);
}

void modifyActuatorValue() {
    sleep((rand() % 2) + 2)

    pthread_mutex_lock(&modifyActuatorValue);
    printf("Alterando: <ATUADOR> com valor <NIVEL DE ATIVIDADE>!\n");
    sleep(1)
    pthread_mutex_unlock(&modifyActuatorValue);

}


int main() {
    //Declaração dos semáforos, não sei se tem como inicializar eles direto, como os mutexes.
    sem_t createSensorialDataSemaphore, readSensorialDataSemaphore;
    //Inicializando os mutexes
    pthread_mutex_t createSensorialDataMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t modifyActuatorValueMutex = PTHREAD_MUTEX_INITIALIZER;
    sem_init(&sensorialDataBufferEmpty, 0, NUM_SENSORS); //Semáforo para produção
    sem_init(&sensorialDataBufferFull, 0, 0); //Semáforo para consumo
    srand(time(NULL));
    
    startDict();

    for(int i = 0; i < NUM_ACTUATORS; i++) {
        printf("Key: %d, Value: %d\n", actuatorsDict[i].key, actuatorsDict[i].value);
    }

    //Destruição dos semáforos e mutexes
    sem_destroy(&sensorialDataBufferEmpty);
    sem_destroy(&sensorialDataBufferFull);
    pthread_mutex_destroy(&createSensorialDataMutex);
    pthread_mutex_destroy(&modifyActuatorValueMutex);
    return 0;
}