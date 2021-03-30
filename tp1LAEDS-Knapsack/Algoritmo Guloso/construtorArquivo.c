#include <stdio.h>
#include <stdlib.h>

void construtorArquivo(){
    int quantItens;
    int capMochila;
    int peso[100000];
    int valor[100000];
    int pesoMax = 0;

    printf("Informe a quantidade de itens 'menor ou igual a 100000': ");
    scanf("%d", &quantItens);

    FILE *f = fopen("arquivo_entrada.txt","w");

    for(int i=0; i<quantItens; i++){
        peso[i] = rand()%9+1;
        valor[i] = rand()%17+2;
        pesoMax += peso[i];
    }

     /*isso garante um uma mochila com capacidade mínima que caiba ao menos um dos itens
      gerados e com capacidade máxima igual a metade da soma do peso dos itens*/
    int magica = (rand()%quantItens)-1;
    capMochila = ( rand() % ((pesoMax/2)-peso[magica])) + peso[magica];

    fprintf(f,"%d\n",capMochila);
    fprintf(f,"%d\n",quantItens);

    for(int i = 0; i<quantItens; i++){
        fprintf(f,("%d %d\n"), peso[i], valor[i]);
    }
    fclose(f);
}

int main(){
    construtorArquivo();
    printf("\n*ARQUIVO CONSTRUIDO COM SUCESSO!*\n");
    return 0;
}
