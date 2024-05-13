## Relatório do Trabalho de Programação Paralela

### Descrição do Problema

O problema abordado neste trabalho consiste na implementação de um sistema de acionamento automático em carros autônomos, utilizando conceitos de programação multithread. O sistema é composto por sensores que coletam dados do ambiente e condições do veículo, atuadores responsáveis por controlar as ações do veículo, e uma central de controle que processa os dados dos sensores e controla os atuadores. O objetivo é explorar padrões de programação multithread, como produtor/consumidor, fork-join e pool de threads, para garantir um desempenho eficiente e tempos de resposta rápidos.

### Implementação

O código foi desenvolvido em linguagem C e utiliza as bibliotecas pthread e OpenMP para criar e gerenciar as threads. Abaixo estão as principais características da implementação:

- **Sensores**: Os sensores são representados por threads individuais que periodicamente geram dados sensoriais aleatórios. Cada sensor é implementado como uma thread separada que executa a função createSensorialData. Dentro dessa função, um dado sensorial aleatório é gerado dentro do intervalo de 0 a 1000, e esse dado é armazenado em um buffer compartilhado entre os sensores. A geração de dados ocorre em intervalos de 1 a 5 segundos, simulados por meio da função sleep. Para garantir a exclusão mútua ao acessar o buffer compartilhado, é utilizado um mutex (createSensorialDataMutex).

- **Central de Controle**: A central de controle é implementada como uma única thread que opera sobre várias unidades de processamento usando OpenMP. Essa thread é responsável por ler os dados sensoriais do buffer compartilhado, processá-los e controlar os atuadores correspondentes. A central executa duas tarefas em paralelo:

    1. **Modificação do nível de atividade do atuador**: A central define um novo valor de atividade para o atuador correspondente ao dado sensorial recebido. Para garantir a integridade dos dados, é utilizado um mutex (`actuatorsMutex`) para controlar o acesso concorrente aos atuadores.

    2. **Envio da modificação para o painel do veículo**: A central envia a modificação do atuador para o painel do veículo, simulado pela função `writeOnScreen`. Esta função também é executada em paralelo e utiliza um mutex (`writeOnScreenMutex`) para garantir a exclusão mútua ao acessar o painel do veículo.

- **Atuadores**: Os atuadores são representados por uma tabela compartilhada, onde cada atuador é associado a um nível de atividade. O acesso aos atuadores é gerenciado por mutexes para garantir a exclusão mútua durante a modificação do nível de atividade. A função `modifyActuatorValue` é responsável por modificar o valor do atuador e é executada em uma thread separada.

- **Semaforos e Mutexes**: Foram utilizados semáforos para controlar o acesso ao buffer de dados sensoriais (`sensorialDataBufferEmpty` e `sensorialDataBufferFull`). Os mutexes foram utilizados para garantir a exclusão mútua em seções críticas do código, como a criação de novos dados sensoriais (`createSensorialDataMutex`), acesso aos atuadores (`actuatorsMutex`), e escrita no painel do veículo (`writeOnScreenMutex`).

### Compilação e Execução

Para compilar o código, utilize o seguinte comando: ```gcc main.c -o main -pthread -fopenmp```

Para executar o programa, utilize o seguinte comando: ```./main```


### Resultados...
