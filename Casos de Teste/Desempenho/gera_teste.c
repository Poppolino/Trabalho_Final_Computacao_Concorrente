#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>

#define LONG long long int



// Gera um número inteiro aleatório dentro do intervalo definido pelo início e fim
int num_aleatorio(int ini, int fim){
    static int semente = 0;


    /* Define uma vez a semente usada pelo gerador pseudo-aleatório */
    if(!semente){
        LONG tempo_seg = (LONG) time(NULL);
        srand(tempo_seg % UINT_MAX);
        semente = 1;
    }

    return ini + (rand() % (fim-ini+1));     
}



int main(int argc, char *argv[]){
	FILE *arq;
	char *nome_arq;
	int vertices, arestas, a, b, peso;

	
	/* Encerra o programa se tiver sido executado com pouca informação */
	if(argc < 4){
		printf("Escreva o padrão: %s <Número de Vértices> <Número de arestas> <Nome do Novo Arquivo>\n\n", argv[0]);
		return 1;
	}
	
	
	/* Converte o dados de entrada para valores inteiros e pega o nome do arquivo */
	vertices = atoi(argv[1]);
	arestas = atoi(argv[2]);
	nome_arq = argv[3];

	
	/* Se não houver vértices ou se tiver dimensão negativa, termina o programa */
    if(vertices <= 0 || arestas < 0){
        printf("O grafo deve ter pelo menos um vértice e o número de arestas deve ser não negativo.\n\n"); 
        return 1;
    }
	
	
	/* Abre um arquivo para escrita */
	arq = fopen(nome_arq, "w");
	
	/* Encerra o programa caso não consiga criar o arquivo */
	if(arq == (FILE *)NULL){
		printf("Problema na criação do arquivo.\n\n");
		return 2;
	}


	/* Escreve o número de vértices e de arestas no arquivo */
	fprintf(arq, "%d %d\n", vertices, arestas);


	/* 
		Gera aleatoriamente as arestas, evitando pesos negativos 
		para não gerar ciclos negativos.
	*/
	for(int i=0; i<arestas; i++){
		a = num_aleatorio(1, vertices);
		b = num_aleatorio(1, vertices);
		peso = num_aleatorio(1, arestas);
		
		fprintf(arq, "%d %d %d\n", a, b, peso);
	}


	/* Fecha o arquivo de escrita */
	fclose(arq);

	return 0;
}
