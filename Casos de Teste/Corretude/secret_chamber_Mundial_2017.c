#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LONG long long int
#define INF LONG_MAX
#define min(a,b) (a<b? a:b)



LONG **leitura(int *pares){
    int m, n;
    char a, b;
    LONG **mat; 

    scanf("%d%d", &m, &n);


    mat = (LONG **) malloc(26 * sizeof(LONG *));
    
    if(mat == (LONG **)NULL)
        exit(1);
    

    for(int i=0; i < 26; i++){
        mat[i] = (LONG *) malloc(26 * sizeof(LONG));

        if(mat[i] == (LONG *)NULL)
            exit(1); 

        for(int j=0; j < 26; j++)
            mat[i][j] = INF;  
        
        mat[i][i] = 0;  
    }


    for(int i=0; i<m; i++){
        scanf(" %c %c", &a, &b);
        mat[a - 'a'][b - 'a'] = 0;
    }


    *pares = n;
    return mat;
}



double GET_TIME(){
    return 0;
}



double floyd_warshall_sequencial(LONG *mat[], int n){
    double inicio, fim;
    
    inicio = GET_TIME();
    

    for(int k=0; k<n; k++){
        for(int i=0; i<n; i++){
            if(k == i) 
                continue;

            for(int j=0; j<n; j++){

                if(mat[i][k] == INF || mat[k][j] == INF)
                    continue;

                mat[i][j] = min(mat[i][j], mat[i][k] + mat[k][j]);
            }
        }
    }
    

    fim = GET_TIME();
    
    return fim - inicio;
}




int iguais(LONG *mat[], char pal_A[], char pal_B[]){
    int a, b;

    if(strlen(pal_A) != strlen(pal_B))
        return 0;

    for(int i=0; pal_A[i] != '\0'; i++){
        a = pal_A[i] - 'a';
        b = pal_B[i] - 'a';

        if(mat[a][b] == INF)
            return 0;
    }

    return 1;
}



int main(){
    int pares;
    LONG **mat;
    char pA[51], pB[51];


    mat = leitura(&pares);

    floyd_warshall_sequencial(mat, 26);


    for(int i=0; i<pares; i++){ 
        scanf(" %s %s", pA, pB);

        if(iguais(mat, pA, pB))
            printf("yes\n");
        else
            printf("no\n");
    }
    

    for(int i=0; i < 26; i++)
        free(mat[i]);  
    
    free(mat);

    return 0;
}
