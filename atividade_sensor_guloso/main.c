#include "vacuum.h"
#include <stdio.h>

int main() {
    // Instancia o ambiente com 7 linhas e 10 colunas
    enviroment E = newEnviroment(7, 10);
    // Essa linha coloca 12 sujeiras em pontos aleatórios do nosso ambiente
    initEnviromentDirt(E, 12);

    // Instancia o aspirador de pó
    cleaner C = newCleaner(E);

    // Essa linha executa o agente (aspirador de pó)
    runAgent(&C, E);

    // Essa linha deleta o ambiente ao final da execução do aspirador
    delEnviroment(&E);
    return 0;
}
