/************************************************
LAED1 - Trabalho Pratico 1
Aluno: Fulvio Taroni Monteforte
       Thiago Lima Bahia Santos
Matricula: 20183005944
           20183000302
Descricao do programa: Problema da Mochila | Algoritmo Guloso + gettimeofday()
Data: 24/09/19
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_TAM 100000

struct Item{
    int selecionado;
    int id;
    int valor;
    int peso;
    float razao;
};
typedef struct Item item;


void algoritmoGuloso(item itens[], int quantItens, int capMochila, int *pesoMax, int *valorMax){
    int mochila = 0;
    int tempPeso = 0;
    /*Com o vetor de itens já ordenado, ele seleciona as posições da esqquerda para direita
      até que a soma dos pesos chegue em seu limite*/
    for(int i = 0; i < quantItens; i++){
        if((itens[i].peso + tempPeso) <= capMochila){
            tempPeso += itens[i].peso;
            mochila += itens[i].valor;
            itens[i].selecionado = 1;
        }else break;
    }
    *pesoMax = tempPeso;
    *valorMax = mochila;
}


void ordenarEstrutra(item itens[], int quantItens){
    for(int i = 0; i < quantItens; i++){
        itens[i].razao = (float)itens[i].valor/itens[i].peso;
    }

    //Método inserction sort para ordenação de vetores, ineficiente para vetores grandes
    item aux;
    for(int i = 0; i < quantItens; i++){
        for(int j = 0; j < quantItens; j++){
            if(itens[i].razao > itens[j].razao){
                aux = itens[i];
                itens[i] = itens[j];
                itens[j] = aux;
            }
        }
    }
}


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


void carregarArquivo(int *capMochila, int *quantItens, item itens[]){
    FILE *f;
    f = fopen("arquivo_entrada.txt","r");

    int cont = 0;

    if(f == NULL){
    printf("Problema na abertura do arquivo!\n");
    exit(1);
    }
    fscanf(f,"%d",capMochila);
    fscanf(f,"%d",quantItens);

    while(fscanf(f,"%d %d", &itens[cont].peso, &itens[cont].valor)!=EOF){
    itens[cont].id = cont;
    itens[cont].selecionado = 0;
    cont++;
    }
    fclose(f);
}

void imprimirArquivo(item itens[], int quantItens, int maxPeso, int maxValor){

    FILE *f = fopen("arquivo_resposta.txt","w");

    if (f == NULL) {
          printf("Erro na abertura do arquivo!\n");
          exit(1);
    }

    for (int i=0; i<quantItens; i++)
        if (itens[i].selecionado == 1) {
            fprintf (f, "%d %d %d\n", itens[i].id, itens[i].peso, itens[i].valor);
        }
    fprintf (f, "%d\n%d", maxPeso, maxValor);
    fclose(f);
}


/* Teste da medicao do tempo*/
int main(int argc, char *argv[])
{
     // Variaveis relacionadas com a medicao do tempo:

     struct timeval inicio, fim;
     /*estrutura que armazena o tempo total que o programa gasta, relaciona-se com
     a funcao gettimeofday()*/

     long totalmicroseg, totalseg; //tempo total do programa

     /* armazenam a diferenca entre o tempo inicial e o final, ou seja, o tempo
     total gasto pelo programa todo. */

     //obtendo o tempo em que o programa comeca.
     gettimeofday(&inicio, NULL);

     //coloque aqui o algoritmo
     //--------------------------------------------------------------------------------
     //--------------------------------------------------------------------------------
     int pesoMax, valorMax;
     int capMochila, quantItens;
     item itens[MAX_TAM];

     //Função construtorArquivo() suprimida do código. Utilizar, se necessário.
     //construtorArquivo();
     //Lê o arquivo criado
     carregarArquivo(&capMochila, &quantItens, itens);
     //Ordena o vetor de estrutura do tipo item
     ordenarEstrutra(itens, quantItens);
     //Algoritmo para solucionar o problema
     algoritmoGuloso(itens, quantItens, capMochila, &pesoMax, &valorMax);
     /*Imprime o resultado com o padrão id, peso e valor. Nas últimas 2 linhas é
       informado o peso e valor contido na mochila*/
     imprimirArquivo(itens, quantItens, pesoMax, valorMax);
     //--------------------------------------------------------------------------------
     //--------------------------------------------------------------------------------


     gettimeofday(&fim, NULL); //obtem tempo final do programa
     totalseg = fim.tv_sec - inicio.tv_sec; //diferenca em segundos
     totalmicroseg = fim.tv_usec - inicio.tv_usec; //diferenca em microsegundos

     /*se a diferenca em microssegundos for negativa, os segundos terao que
     emprestar uma unidade; 1 microseg = 10E-6 s. */
     if (totalmicroseg <0)
     { totalmicroseg += 1000000;
      totalseg -= 1;
     };
     printf ("\n");
     printf ("***************************************************************\n");
     printf ("Tempo total: %ld segundos e %ld microssegundos.\n", totalseg, totalmicroseg);
     printf ("***************************************************************\n");
     printf ("\n");
     return(0);
}
