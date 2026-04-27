// Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito
#include "astar.h"

// Ponteiro para função heuristica, definido em searchGame.c
int (*heuristica)(game*);

// Cria o Heap inicializando o vetor
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HeapNode*) malloc(capacity * sizeof(HeapNode));
    return minHeap;
}

// Função auxiliar para trocar dois elementos de posição
void swapHeapNode(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Função que reorganiza a árvore de cima para baixo (usada na extração)
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // Verifica se o filho da esquerda é menor que o pai
    if (left < minHeap->size && minHeap->array[left].f < minHeap->array[smallest].f)
        smallest = left;

    // Verifica se o filho da direita é menor que o menor atual
    if (right < minHeap->size && minHeap->array[right].f < minHeap->array[smallest].f)
        smallest = right;

    // Se o menor não for o pai, troca e continua descendo
    if (smallest != idx) {
        swapHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Insere um novo nó no Heap
void insertMinHeap(MinHeap* minHeap, node n, int f) {
    // Se o vetor encher, dobramos o tamanho dele (realocação dinâmica)
    if (minHeap->size == minHeap->capacity) {
        minHeap->capacity *= 2;
        minHeap->array = (HeapNode*) realloc(minHeap->array, minHeap->capacity * sizeof(HeapNode));
    }

    minHeap->size++;
    int i = minHeap->size - 1;
    minHeap->array[i].n = n;
    minHeap->array[i].f = f;

    // Reorganiza a árvore de baixo para cima: enquanto o filho for menor que o pai, eles trocam
    while (i != 0 && minHeap->array[(i - 1) / 2].f > minHeap->array[i].f) {
        swapHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Remove e retorna o nó com o menor custo (que sempre está na raiz: índice 0)
node extractMin(MinHeap* minHeap) {
    if (minHeap->size <= 0) return NULL;
    
    if (minHeap->size == 1) {
        minHeap->size--;
        return minHeap->array[0].n;
    }

    // Pega a raiz
    node root = minHeap->array[0].n;
    
    // Move o último elemento do vetor para a raiz
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    
    // Reorganiza a árvore
    minHeapify(minHeap, 0);

    return root;
}

/* Implementação de uma função auxiliar recursiva para o algoritmo IDA* (Busca A* com aprofundamento iterativo). 
Ela é chamada pela função IDAStarSearch, e é responsável por explorar a árvore de busca até um certo limite, retornando o custo mínimo encontrado que ultrapassa esse limite, ou a solução caso ela seja encontrada. 
Por ser uma função recursiva, não precisamos armazenar os nós filhos em uma fila, como no A* tradicional. Esse algoritmo é geralmente mais eficiente em termos de memória.
Contudo, a depender da heurística utilizada, ou da configuração inicial do jogo, ele pode demorar muito mais, e até dar segmentation fault se não for implementado corretamente (tivemos alguns erros com isso no início). 
Definimos um limite para o custo, e a cada iteração do IDA* esse limite é atualizado para o menor custo encontrado que ultrapassa o limite anterior. 
Assim, a função recursiva explora a árvore de busca, e retorna a solução caso ela seja encontrada, ou o menor custo encontrado que ultrapassa o limite caso contrário. 
O argumento pecaAnterior é utilizado para evitar que a função explore movimentos que desfazem o movimento anterior, o que pode levar a loops infinitos.
*/

int func_recursiva_auxiliar(node n, int g, int limite, node* solucao, int pecaAnterior) {
    int f = g + heuristica(n->state);
    if (f > limite) return f;

    if (endGame(n->state)) {
        *solucao = n;
        return -1;
    }

    int min = 1000000;

    for (int k = 1; k < 9; k++) {
        if (k == pecaAnterior) continue;

        game* copia = newGame();
        copyGame(n->state, copia);

        if (moveGame(copia, k)) {
            node filho = childNode(n, k);
            copyGame(copia, filho->state);

            int temp = func_recursiva_auxiliar(filho, g + 1, limite, solucao, k);

            if (temp == -1) {
                delGame(copia);
                return -1;
            }

            if (temp < min) min = temp;

            delNode(filho);
        }

        delGame(copia);
    }

    return min;
}

/* Algoritmo A* tradicional que implementamos utilizando um Min-Heap para a fronteira, e uma fila para os nós explorados. Nós escolhemos usar min-heap para reduzir o tempo médio de execução desse algoritmo quando a heurística for a de peças fora do lugar, que é uma heurística mais fraca e tende a gerar uma árvore de busca maior. O A* tradicional tem um desempenho muito melhor com heurísticas mais fortes, como a de Manhattan, onde o número de nós expandidos é muito menor, e o tempo gasto para reorganizar o heap é compensado pela redução do número de nós explorados. (Observação: tinhamos feito usando duas filas, explorados e fronteira, mas isso deixava o algoritmo absurdamente lento quando h(n) = h_fora).
*/

node AStarSearch(game* G){
    node raiz = childNode(NULL, 0);
    copyGame(G, raiz->state);

    // Inicializa o Min-Heap com uma capacidade inicial razoável (ex: 1000)
    MinHeap* fronteira = createMinHeap(1000);
    
    // A lista de explorados continua igual
    fila explorados = newFila(); 
    
    // Insere a raiz calculando o custo f inicial
    int f_inicial = raiz->pathCost + heuristica(raiz->state);
    insertMinHeap(fronteira, raiz, f_inicial);

    while(fronteira->size > 0){
        // Extração instantânea do menor elemento O(1) + reordenação O(log N)
        node pai = extractMin(fronteira);

        // Se este nó já foi explorado (chegamos nele por um caminho mais caro antes), ignora.
        if (isIn(explorados, pai)) {
            // delNode(pai); // Descomente se precisar liberar a memória das repetições
            continue;
        }

        // Checa condição de vitória
        if(endGame(pai->state)) {
            free(fronteira->array);
            free(fronteira);
            return pai;
        }

        // Marca como explorado
        insert(explorados, pai);

        // Gera os filhos
        for (int k=1; k<9; k++){
            game* tempGame = newGame();
            copyGame(pai->state, tempGame);
            
            if (moveGame(tempGame, k)){
                node filho = childNode(pai, k);
                copyGame(tempGame, filho->state);
                
                // Se o filho NÃO está nos explorados, a função calcula o custo e joga no Heap
                if(!isIn(explorados, filho)){
                    int g = filho->pathCost;
                    int h = heuristica(filho->state);
                    insertMinHeap(fronteira, filho, g + h);
                }
                else{
                    delNode(filho);
                }
            }
            delGame(tempGame);
        }
    }
    
    free(fronteira->array);
    free(fronteira);
    return NULL;
}

/* Aqui a implementação do IDA* (Busca A* com aprofundamento iterativo), que é uma variação do A* tradicional que utiliza menos memória e possui menor complexidade. Essa função faz uso da função recursiva auxiliar que já explicamos acima. Aqui, o limite é inicialmente definido como a heurística do nó raiz, e a cada iteração do IDA* esse limite é atualizado para o menor custo encontrado que ultrapassa o limite anterior. Assim, a função recursiva explora a árvore de busca, e retorna a solução caso ela seja encontrada, ou o menor custo encontrado que ultrapassa o limite caso contrário. O argumento pecaAnterior é utilizado para evitar que a função explore movimentos que desfazem o movimento anterior, o que pode levar a loops infinitos.
*/

node IDAStarSearch(game* G) {
    node raiz = childNode(NULL, 0);
    copyGame(G, raiz->state);

    int limite = heuristica(raiz->state);

    while (1) {
        node solucao = NULL;

        int temp = func_recursiva_auxiliar(raiz, 0, limite, &solucao, -1);

        if (temp == -1) return solucao;

        limite = temp;
    }
}