#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define LONG long long int
#define INF LONG_MAX
#define min(a,b) (a<b? a:b)



LONG **leitura(int *dim){
    int n, m, u, v, w;
    LONG **mat; 

    scanf("%d%d", &n, &m);


    mat = (LONG **) malloc(n * sizeof(LONG *));
    
    if(mat == (LONG **)NULL)
        exit(1);
    

    for(int i=0; i < n; i++){
        mat[i] = (LONG *) malloc(n * sizeof(LONG));

        if(mat[i] == (LONG *)NULL)
            exit(1); 

        for(int j=0; j < n; j++)
            mat[i][j] = INF;  
        
        mat[i][i] = 0;  
    }


    for(int i=0; i<m; i++){
        scanf("%d%d%d", &u, &v, &w);
        mat[u][v] = w;
        mat[v][u] = w;
    }


    *dim = n;
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





int main(){
    LONG **mat;
    int dim, maior, menor_geral;


    mat = leitura(&dim);

    floyd_warshall_sequencial(mat, dim);


    for(int i=0; i<dim; i++){   
        maior = mat[i][0];
        
        for(int j=1; j<dim; j++){
            if(maior < mat[i][j])
                maior = mat[i][j];
        }

        if(i==0 || menor_geral > maior)
            menor_geral = maior;
    }

    
    printf("%d\n", menor_geral);
    

    for(int i=0; i < dim; i++)
        free(mat[i]);  
    
    free(mat);

    return 0;
}
