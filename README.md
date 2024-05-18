## Relatório do Trabalho de Programação Paralela

### Descrição do Problema

O problema abordado neste trabalho consiste na implementação de um sistema de acionamento automático em carros autônomos, utilizando conceitos de programação multithread. O sistema é composto por sensores que coletam dados do ambiente e condições do veículo, atuadores responsáveis por controlar as ações do veículo, e uma central de controle que processa os dados dos sensores e controla os atuadores. O objetivo é explorar padrões de programação multithread, como produtor/consumidor, fork-join e pool de threads, para garantir um desempenho eficiente e tempos de resposta rápidos.

### Implementação

O código foi desenvolvido em linguagem C e utiliza as bibliotecas pthread e OpenMP para criar e gerenciar as threads. Abaixo estão as principais características da implementação:

- **Sensores**: Os sensores são representados por threads individuais que periodicamente geram dados sensoriais aleatórios. Cada sensor é implementado como uma thread separada que executa a função `createSensorialData`. Dentro dessa função, um dado sensorial aleatório é gerado dentro do intervalo de 0 a 1000, e esse dado é armazenado em um buffer compartilhado entre os sensores. A geração de dados ocorre em intervalos de 1 a 5 segundos, simulados por meio da função `sleep`. Para garantir a exclusão mútua ao acessar o buffer compartilhado, é utilizado um mutex (`createSensorialDataMutex`).

- **Central de Controle**: A central de controle é responsável por ler os dados sensoriais do buffer compartilhado e preparar as tarefas para os atuadores. Esta central não processa diretamente os dados dos atuadores, mas organiza as tarefas para serem executadas por um pool de threads simulado.

- **Buffer para Atividades**: Um buffer de tamanho fixo (`BUFFER_SIZE`) foi implementado para armazenar as atividades de controle. Esse buffer é gerenciado por um mutex (`bufferMutex`) e dois semáforos (`sensorialDataBufferEmpty` e `sensorialDataBufferFull`) para controlar o acesso de leitura e escrita.

- **Simulação de Pool de Threads**: Uma thread (`simulatedThreadPoolThread`) foi criada para simular um pool de threads. Esta thread processa os dados do buffer de atividades em paralelo utilizando OpenMP, representando uma abordagem eficiente para lidar com múltiplas tarefas simultaneamente. Cada thread no pool lê uma atividade do buffer e processa a modificação dos atuadores de forma concorrente.

- **Atuadores**: Os atuadores são representados por uma tabela compartilhada, onde cada atuador é associado a um nível de atividade. O acesso aos atuadores é gerenciado por mutexes para garantir a exclusão mútua durante a modificação do nível de atividade. A função `modifyActuatorValue` é responsável por modificar o valor do atuador e é executada em uma thread separada.

- **Semáforos e Mutexes**: Foram utilizados semáforos para controlar o acesso ao buffer de dados sensoriais (`sensorialDataBufferEmpty` e `sensorialDataBufferFull`) e ao buffer de atividades (`activityBuffer`). Os mutexes foram utilizados para garantir a exclusão mútua em seções críticas do código, como a criação de novos dados sensoriais (`createSensorialDataMutex`), acesso aos atuadores (`actuatorsMutex`), e escrita no painel do veículo (`writeOnScreenMutex`).

### Compilação e Execução

Para compilar o código, utilize o seguinte comando:

```sh
gcc new_main.c -o main -pthread -fopenmp
```

### Casos de Teste e Resultados Esperados

#### Caso de Teste 1: Configuração Padrão

**Configuração:**
- Número de sensores: 4
- Número de atuadores: 4
- Probabilidade de falha ao modificar um atuador ou durante o processo: 20%

**Execução:**
Executar o código conforme fornecido sem alterações.

**Resultado Esperado:**
- Sensores gerarão dados sensoriais periodicamente.
- O centro de controle lerá os dados dos sensores e distribuirá tarefas para os atuadores.
- O thread pool simulado modificará os valores dos atuadores, com falhas ocasionais (20% de chance de falha em qualquer etapa).
- A saída deve incluir linhas indicando a alteração dos atuadores e falhas ocasionais.

**Resultado Obtido:**
```c
Alterando: <3> com valor <35>!
Falha: <3>
Falha: <4>
Alterando: <4> com valor <37>!
Alterando: <1> com valor <63>!
Alterando: <3> com valor <96>!
Alterando: <2> com valor <15>!
Alterando: <4> com valor <34>!
Falha: <3>
Alterando: <3> com valor <72>!
```
---

#### Caso de Teste 2: Aumentar o Número de Sensores e Atuadores Mantendo a Falha de 20%

**Configuração:**
- Número de sensores: 10
- Número de atuadores: 10

**Modificação no Código:**
```c
#define NUM_SENSORS 10
#define NUM_ACTUATORS 10
```
### Execução:

Executar o código com a nova configuração.

**Resultado Esperado:**

- Sensores adicionais gerarão mais dados sensoriais.
- O centro de controle distribuirá as tarefas de forma mais frequente para um conjunto maior de atuadores.
- A saída deve incluir mais alterações e falhas, refletindo o aumento na atividade do sistema.

**Resultado Obtido:**
```c
Alterando: <5> com valor <39>!
Alterando: <4> com valor <55>!
Alterando: <7> com valor <39>!
Alterando: <5> com valor <22>!
Falha: <9>
Alterando: <6> com valor <31>!
Alterando: <2> com valor <88>!
Falha: <10>
Alterando: <5> com valor <73>!
Alterando: <6> com valor <38>!
```

---

#### Caso de Teste 3: Aumentando a probabilidade de falha

**Configuração:**

- Probabilidade de falha ao modificar um atuador ou durante o processo: 50%

**Modificação no Código:**

Modificar a função `modifyActuatorValue` para reduzir a probabilidade de falha:

```c
void modifyActuatorValue(int data, int actuator) {
    if (rand() % 2 == 0) {  // 50% chance de falha
        exit(1);
    } else {
        sleep((rand() % 2) + 2);
        actuatorsValue[actuator] = data;
        exit(0);
    }
}
```
Modificar outro techo também:
```c
else {
    if (rand() % 2 == 0) {  // 50% chance de falha
        processStatus = 1;  
    }
    wait(&childProcessStatus);
    ...
}
```

**Resultado Esperado:**

- A probabilidade de falhas durante a modificação dos atuadores ou em qualquer etapa do processo será de 50%.
- A saída deve incluir mais mensagens de falha, refletindo o aumento significativo na probabilidade de falha em comparação com a configuração padrão.

**Resultado Obtido:**
```c
Falha: <8>
Falha: <1>
Alterando: <3> com valor <1>!
Alterando: <3> com valor <57>!
Falha: <5>
Falha: <3>
Falha: <10>
Alterando: <6> com valor <10>!
Alterando: <3> com valor <26>!
Alterando: <4> com valor <73>!
```

### Conclusão

Os resultados dos testes demonstraram que a implementação do sistema de acionamento automático em carros autônomos, utilizando programação multithread em C, é capaz de lidar eficientemente com diferentes configurações e cenários, mantendo a integridade das operações mesmo em condições de alta carga e probabilidade de falha. A utilização de múltiplas threads, semáforos e mutexes proporcionou uma execução concorrente e segura do sistema, garantindo tempos de resposta rápidos e uma distribuição eficiente das tarefas entre os componentes do sistema. Esses resultados validam a abordagem adotada e destacam a viabilidade do uso de técnicas de programação paralela na construção de sistemas complexos e robustos.
