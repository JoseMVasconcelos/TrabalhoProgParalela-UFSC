#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    //Intervalo para conseguir um novo dado sensorial entre 1 a 5 segundos
    sleep((rand() % 5) + 1);
}

void readSensorialData() {
    pass;
}

void controlCenter() {
    // SEMAFORO AQUI
    int sensorialData = readSensorialData();
    new thread 
}

void modifyActuatorValue() {
    sleep((rand() % 2) + 2)

    //mutex lock aqui
    printf("Alterando: <ATUADOR> com valor <NIVEL DE ATIVIDADE>!\n");
    sleep(1)
    //mutex unlock aqui

}



int main() {
    srand(time(NULL));
    startDict();

    for(int i = 0; i < NUM_ACTUATORS; i++) {
        printf("Key: %d, Value: %d\n", actuatorsDict[i].key, actuatorsDict[i].value);
    }

    return 0;
}