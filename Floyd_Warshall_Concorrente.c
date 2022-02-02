/* Trabalho de Implementação */
/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Autores: Sylviane Oliveira e Pedro Poppolino */
/* Codigo: Implementaçao do Algoritmo de Floyd-Warshall de forma sequencial e concorrente (utilizando barreira) */

/*
    * Entrada: Número de threads e arquivo com informações do grafo direcionado e ponderado.
    * O arquivo contém o número de vértices e de arestas (M), seguidas de M linhas com as informações das arestas 
    * (vértice de partida, vértice de chegada e peso da aresta). 
    *
    * Saída: Mensagem informativa sobre a validação do resultado concorrente em relação ao resultado sequencial, 
    * além dos respectivos tempos de execução de cada algoritmo.
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>

#define LONG long long int
#define INF LONG_MAX
#define min(a,b) (a<b? a:b)


int n_threads, dim, cont = 0;
pthread_mutex_t tranca;
pthread_cond_t cond_bar;
LONG **mat;





/* Realiza a leitura dos dados e a criação da Matriz de Adjacências */
LONG **leitura(int argc, char *argv[], int *n_threads, int *dim){
    FILE *arq;
    LONG dist;
    char *nome_arq;
    int a, b, vertices, arestas; 
    
    
    /* Encerra o programa se tiver sido executado com pouca informação */
    if(argc < 3){
        printf("Escreva o padrão: %s <Número de Threads> <Caminho do Arquivo de TEXTO>\n\n", argv[0]);
        printf("Na primeira linha do arquivo, deve ter o número de vértices e o número de arestas (M), respectivamente.\n\n");
        printf("Em cada uma das M linhas seguintes, devemos ter, respectivamente, 3 inteiros: \n");
        printf("\t1) O índice do vértice de partida; \n\t2) O índice do vértice de chegada; \n\t3) E a \"distância\" entre eles.\n\n");
        printf("NÃO ENTRE com um grafo que possua ciclo negativo, pois o algoritmo não funciona para esse caso.\n\n");
        printf("OBS: Os vértices devem ser numerados em sequência, começando de 1.\n");
        printf("     As arestas sempre são consideradas como direcionadas.\n"); 
        printf("     Se quiser uma aresta bidirecional, liste-a duas vezes com os vértices de partida e chegada trocados.\n\n");
        
        exit(1);
    }
    
    
    /* Converte o dado de entrada para um valor inteiro e pega o nome do arquivo */
    *n_threads = atoi(argv[1]);
    nome_arq = argv[2];
    
    /* Se não houver threads, não tem como executar o programa */
    if(*n_threads < 1){
        printf("O número de Threads deve ser um inteiro maior que zero.\n\n");
        exit(1);
    }
    
    
    /* Abre o arquivo para leitura */
    arq = fopen(nome_arq, "r");
    
    /* Encerra o programa caso não consiga ler ou não tenha nada no arquivo */
    if(arq == (FILE *)NULL || fscanf(arq, "%d%d", &vertices, &arestas) == EOF){
        printf("Problema na leitura do arquivo: inexistente ou vazio.\n\n");
        exit(2);
    }
    
        
    /* Se não houver vértices ou se tiver dimensão negativa, termina o programa */
    if(vertices <= 0 || arestas < 0){
        printf("O grafo deve ter pelo menos um vértice e o número de arestas deve ser não negativo.\n\n"); 
        exit(1);
    }
    
        
    /* Evita criar mais threads que linhas da matriz */
    if(*n_threads > vertices){
        printf("Número de Threads diminuído para o número de vértices do grafo.\n\n");
        *n_threads = vertices;
    }  
    
    
    
    /* Tenta alocar memória para a matriz. Se não conseguir, termina o programa. */
    mat = (LONG **) malloc(vertices * sizeof(LONG *));
    
    if(mat == (LONG **)NULL){
        printf("Problema na alocação de memória da matriz.\n\n");
        exit(2);
    }
    

    for(int i=0; i < vertices; i++){
        /* Tenta alocar memória para a linha da matriz. Se não conseguir, termina o programa. */
        mat[i] = (LONG *) malloc(vertices * sizeof(LONG));

        if(mat[i] == (LONG *)NULL){
            printf("Problema na alocação de memória da matriz.\n\n");
            exit(2);
        } 

        
        /* Inicializa a matriz com distância infinita, ou seja, sem caminho entre os vértices */
        for(int j=0; j < vertices; j++)
            mat[i][j] = INF;  
        
        /* 
            Considera a distância para ele mesmo como zero, 
            mas se houver aresta para si próprio, vai ser sobrescrito depois.
        */
        mat[i][i] = 0;  
    }
    
    
    
    /* 
        Coloca a distância obtida na posição correspondente da matriz. 
        Se os limites não forem respeitados, termina o programa. 
    */
    for(int i=0; i < arestas; i++){
        if( fscanf(arq, "%d%d%lld", &a, &b, &dist) == EOF ){
            printf("Número de linhas menor que o número de arestas.\n\n");
            exit(1);
        }
        
        if(a<1 || b<1 || a>vertices || b>vertices){
            printf("Os índices devem ir de 1 ao número de vértices.\n\n");
            exit(1); 
        }
        
        mat[a-1][b-1] = dist;
    }
    
    
    if( fscanf(arq, "%d%d%lld", &a, &b, &dist) != EOF ){
        printf("Número de linhas maior que o número de arestas.\n\n");
        exit(1);
    }
    
    
    /* Fecha o arquivo de leitura, define a dimensão da matriz gerada e a retorna */
    fclose(arq);
    *dim = vertices;
    
    return mat;
}





/* Faz uma cópia da matriz de entrada para realizar o algoritmo de forma sequencial */
LONG **gera_matriz_sequencial(LONG *mat[], int dim){
    LONG **mat_seq;
    

    /* Tenta alocar memória para a matriz. Se não conseguir, termina o programa. */
    mat_seq = (LONG **) malloc(dim * sizeof(LONG *));
    
    if(mat_seq == (LONG **)NULL){
        printf("Problema na alocação de memória da matriz.\n\n");
        exit(2);
    }
    

    for(int i=0; i < dim; i++){
        /* Tenta alocar memória para a linha da matriz. Se não conseguir, termina o programa. */
        mat_seq[i] = (LONG *) malloc(dim * sizeof(LONG));
    
        if(mat_seq[i] == (LONG *)NULL){
            printf("Problema na alocação de memória da matriz.\n\n");
            exit(2);
        }

        /* Copia a linha da matriz original para a nova matriz */
        for(int j=0; j < dim; j++)
            mat_seq[i][j] = mat[i][j];
    }


    /* Retorna a cópia da matriz */
    return mat_seq;
}





/* Pega o tempo atual em segundos */
double GET_TIME(){
    struct timespec tempo;
    
    if( clock_gettime(CLOCK_MONOTONIC, &tempo) ){
        printf("Erro durante a medição do tempo.\n\n");
        exit(3);
    }
    
    return tempo.tv_sec + tempo.tv_nsec/1000000000.0;
}





/* 
    Dada uma Matriz de Adjacências com os valores iniciais de distância entre cada par de vértices,
    aplica o Algoritmo de Floyd-Warshall sequencial para encontrar as menores distâncias entre cada 
    par de vértices, usando quaisquer vértices como intermediários.

    Obs: Esse algoritmo não funciona para grafos com ciclos negativos.
*/
double floyd_warshall_sequencial(LONG *mat[], int n){
    double inicio, fim;
    
    inicio = GET_TIME();
    

    /* Pega o vértice intermediário da vez */
    for(int k=0; k<n; k++){

        /* Calcula as menores distâncias de cada par de vértices, podendo passar pelo intermediário */
        for(int i=0; i<n; i++){
            /* 
                Não precisa calcular para a linha do vértice intermediário, 
                já que nenhuma distância, a partir desse vértice, será alterada nessa etapa.
            */
            if(k == i) 
                continue;

            for(int j=0; j<n; j++){
                /* Se for INF, não há caminho entre os vértices que passe pelo intermediário */
                if(mat[i][k] == INF || mat[k][j] == INF)
                    continue;

                /* 
                    Como há caminho, verifica se o que passa pelo intermediário 
                    é menor que o caminho já calculado, pegando o menor deles. 
                */
                mat[i][j] = min(mat[i][j], mat[i][k] + mat[k][j]);
            }
        }
    }
    

    fim = GET_TIME();
    
    /* Retorna o tempo gasto pela execução sequencial */
    return fim - inicio;
}





/* Define uma sincronização coletiva entre as threads usando barreira */
void barreira(){
    pthread_mutex_lock(&tranca);

    cont++;
    
    if(cont == n_threads){
        cont = 0;
        pthread_cond_broadcast(&cond_bar);
    }
    else
        pthread_cond_wait(&cond_bar, &tranca);

    pthread_mutex_unlock(&tranca);
}





/* Tarefa que cada thread do Floyd-Warshall concorrente terá que fazer */
void *tarefa (void *arg) {
    int id = *((int *) arg);

    /* Pega o vértice intermediário da vez */
    for(int k=0; k < dim; k++) {

        /* A thread é destina a executar sobre certas linhas da matriz, dependendo do seu id */
        for(int i = id; i < dim; i += n_threads){   
            /* 
                Não precisa calcular para a linha do vértice intermediário, 
                já que nenhuma distância, a partir desse vértice, será alterada nessa etapa.
            */
            if(k == i) 
                continue;

            for(int j = 0; j < dim; j++){
                /* Se for INF, não há caminho entre os vértices que passe pelo intermediário */
                if(mat[i][k] == INF || mat[k][j] == INF)
                    continue;
                
                /* 
                    Como há caminho, verifica se o que passa pelo intermediário 
                    é menor que o caminho já calculado, pegando o menor deles. 
                */
                mat[i][j] = min(mat[i][j], mat[i][k] + mat[k][j]);
            }
        }       

        /* 
            Sincronização condicional por barreira. 
            Ela impedirá que as threads usem intermediários distintos simultaneamente.
        */
        barreira();
    }
    
    pthread_exit(NULL);
}





/*
    Cria as threads que vão realizar o Floyd-Warshall concorrente
    e espera elas terminarem para retornar o tempo gasto pela execução. 
*/
double floyd_warshall_concorrente(LONG **mat, int n_threads){
    int *id, erro;
    pthread_t *threads;
    double inicio, fim;
    

    inicio = GET_TIME();
    
    /* Inicializa a variável de exclusão mútua e a de condição da barreira */
    pthread_mutex_init(&tranca, NULL);
    pthread_cond_init(&cond_bar, NULL);
    
    
    /* Inicializa o vetor das threads e termina o programa caso não consiga */
    threads = (pthread_t *) malloc(n_threads * sizeof(pthread_t));
    
    if(threads == (pthread_t *)NULL){
        puts("Problema na alocação de memória do vetor.\n\n"); 
        exit(2);
    }


    /* Inicializa o vetor de id das threads e encerra o programa caso não consiga */
    id = (int *) malloc(n_threads * sizeof(int));

    if(id == (int *)NULL){
        puts("Problema na alocação de memória do vetor.\n\n"); 
        exit(2);
    }


    /* Cria as threads e termina o programa em caso de erro */
    for(int i=0; i < n_threads; i++){
        id[i] = i;
        
        erro = pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]);
        
        if(erro){
            printf("Erro na criação das Threads.\n\n"); 
            exit(4);
        }
    }


    /* Espera todas as threads completarem e encerra o programa em caso de erro */
    for(int i = 0; i < n_threads; i++){
        erro = pthread_join(threads[i], NULL);
    
        if(erro){
            printf("Erro na espera das Threads.\n\n"); 
            exit(4);
        }
    }
    
    
    /* Libera a memória dos vetores das threads */
    free(threads);
    free(id);
    
    
    /* Destrói as variáveis de exclusão mútua e condicional */
    pthread_mutex_destroy(&tranca);
    pthread_cond_destroy(&cond_bar);
    
    
    fim = GET_TIME();

    /* Retorna o tempo gasto pela execução concorrente */
    return fim - inicio;
}





/*
    Função que verifica a igualdade entre a matriz concorrente e a sequencial, 
    assim verificando a corretude do algoritmo concorrente.
*/
int verifica_corretude(LONG *mat[], LONG *mat_seq[], int dim){
    
    for(int i=0; i < dim; i++){
        for(int j=0; j < dim; j++){
            
            /* Se uma posição não for igual, as matrizes diferem */
            if(mat[i][j] != mat_seq[i][j])
                return 0;
        }
    }
    

    /* Se todas as posições forem iguais, as matrizes serão iguais */
    return 1;
} 





/* Função Principal da Aplicação */
int main(int argc, char *argv[]){
    LONG **mat_seq;
    double tempo_conc, tempo_seq;

    
    /* Faz a leitura do grafo e cria a matriz de adjacências */
    mat = leitura(argc, argv, &n_threads, &dim);
    
    /* Copia a matriz criada para uma nova, que será usada no algoritmo sequencial */
    mat_seq = gera_matriz_sequencial(mat, dim);
    
    
    /* Executa os algoritmos de Floyd-Warshall sequencial e concorrente, pegando o tempo gasto */
    tempo_seq = floyd_warshall_sequencial(mat_seq, dim);
    tempo_conc = floyd_warshall_concorrente(mat, n_threads);
    
    
    /* Imprime na tela os tempos gastos por cada algoritmo */
    printf("Tempo gasto pelo Algoritmo Sequencial: %lf\n", tempo_seq);
    printf("Tempo gasto pelo Algoritmo Concorrente: %lf\n\n", tempo_conc);
    
    
    /* Verifica se o algoritmo concorrente obteve uma resposta correta */
    if(verifica_corretude(mat, mat_seq, dim))
        printf("O algoritmo concorrente teve a mesma resposta que o sequencial! :)\n\n");
    else
        printf("O algoritmo concorrente não teve a mesma resposta que o sequencial... :(\n\n");
    
    
    /* Libera as posições de memória das matrizes */
    for(int i=0; i < dim; i++){
        free(mat[i]);  
        free(mat_seq[i]);
    }
    
    free(mat);
    free(mat_seq);
    
    return 0;
}
