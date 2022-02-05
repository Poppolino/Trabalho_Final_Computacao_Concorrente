#include<stdio.h>


/* 
    Descrição: 

    Dado um Caso de Teste do problema "Reunião", da OBI de 2010, 
    gera um arquivo válido para o programa que realiza o Floyd-Warshall Concorrente. 
*/



int main(int argc, char *argv[]){
    int vertices, arestas, a, b, peso;
    FILE *arq_orig, *arq_novo;
    char *nome_arq_orig, *nome_arq_novo;

    
    /* Encerra o programa se tiver sido executado com pouca informação */
    if(argc < 3){
        printf("Escreva o padrão: %s <Caminho do Arquivo Original> <Nome do Novo Arquivo>\n", argv[0]);
        printf("O arquivo de entrada deve ser um Caso de Teste do problema \"Reunião\", da OBI de 2010.\n\n");
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


    /* Lê a quantidade de vértices e arestas do arquivo original */
    fscanf(arq_orig, "%d%d", &vertices, &arestas);

    /* 
        Escreve o número de vértices e de arestas no novo arquivo. 
        Duplica-se o número de arestas porque elas eram bidirecionais no original. 
    */
    fprintf(arq_novo, "%d %d\n", vertices, 2*arestas);


    /* 
        As arestas são lidas do arquivo original, os índices dos vértices são acrescidos de um 
        e as arestas são escritas duas vezes no novo arquivo, uma vez para cada sentido. 
    */
    for(int i=0; i<arestas; i++){
       fscanf(arq_orig, "%d%d%d", &a, &b, &peso);

        a++;
        b++;
        
        fprintf(arq_novo, "%d %d %d\n", a, b, peso);
        fprintf(arq_novo, "%d %d %d\n", b, a, peso);
    }


    /* Fecha os arquivos abertos */
    fclose(arq_orig);
    fclose(arq_novo);

    return 0;
}
