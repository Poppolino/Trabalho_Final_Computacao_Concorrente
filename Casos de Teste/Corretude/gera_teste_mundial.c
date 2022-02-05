#include<stdio.h>


/* 
    Descrição: 

    Dado um Caso de Teste do problema "Secret Chamber at Mount Rushmore", da Final Mundial da Maratona de 2017, 
    gera um arquivo válido para o programa que realiza o Floyd-Warshall Concorrente. 
*/



int main(int argc, char *argv[]){
    int arestas, a, b;
    char letra_1, letra_2; 
    FILE *arq_orig, *arq_novo;
    char *nome_arq_orig, *nome_arq_novo;

    
    /* Encerra o programa se tiver sido executado com pouca informação */
    if(argc < 3){
        printf("Escreva o padrão: %s <Caminho do Arquivo Original> <Nome do Novo Arquivo>\n", argv[0]);
        printf("O arquivo de entrada deve ser um Caso de Teste do problema \"Secret Chamber at Mount Rushmore\", do Mundial da Maratona de 2017.\n\n");
        return 1;
    }
    
    
    /* Pega os nomes dos arquivos */
    nome_arq_orig = argv[1];
    nome_arq_novo = argv[2];
    
    
    /* Abre o arquivo original para leitura */
    arq_orig = fopen(nome_arq_orig, "r");

    /* Encerra o programa caso não consiga ler o arquivo original */
    if(arq_orig == (FILE *)NULL){
        printf("Problema na leitura do arquivo.\n\n");
        return 2;
    }


    /* Abre um novo arquivo para escrita */
    arq_novo = fopen(nome_arq_novo, "w");

    /* Termina o programa caso não consiga criar um novo arquivo */
    if(arq_novo == (FILE *)NULL){
        printf("Problema na criação do arquivo.\n\n");
        return 2;
    }


    /* Lê a quantidade de arestas do arquivo original, ignorando o segundo parâmetro (das consultas) */
    fscanf(arq_orig, "%d%*d", &arestas);

    /* 
        Escreve o número de vértices e de arestas no novo arquivo. 
        Torna-se explícito que sempre são 26 vértices (por serem 26 letras). 
    */
    fprintf(arq_novo, "%d %d\n", 26, arestas);


    /* 
        As arestas são lidas do arquivo original, os índices dos vértices são transformados 
        em inteiros, o peso das arestas é definido como zero e as arestas são escritas no novo arquivo. 
        Demos um valor fixo às arestas porque o peso em si não importa no problema original, poderíamos 
        ter definido outros números positivos no lugar.
    */
    for(int i=0; i<arestas; i++){
       fscanf(arq_orig, " %c %c", &letra_1, &letra_2);

        a = (letra_1 - 'a') + 1;
        b = (letra_2 - 'a') + 1;
        
        fprintf(arq_novo, "%d %d %d\n", a, b, 0);
    }


    /* Fecha os arquivos abertos */
    fclose(arq_orig);
    fclose(arq_novo);

    return 0;
}