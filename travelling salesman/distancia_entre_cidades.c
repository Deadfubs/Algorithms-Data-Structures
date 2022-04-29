#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INFINITO 99999
#define MAX 100

/* O programa está funcionando fazendo a impressão certa no terminal porém todos os casos estão dando errado no runcodes
    :( */

/*A parte que propriamente executa o algoritmo de Djikstra foi feita com base no algoritmo do site:
https://www.thecrazyprogrammer.com/2014/03/dijkstra-algorithm-for-finding-shortest-path-of-a-graph.html*/
typedef struct cidade{
    char nome_cidade[20];
}Cidade;

//Arco é uma estrutura do tipo dois vértices ligados por uma aresta
typedef struct arco{
    char nome_cidade_origem[20];
    char nome_cidade_destino[20];
    int distancia;
}Arco;

void djstriks(int **G, int quant_cidades,int origem, int destino, Cidade *cidades){

	int distancias[quant_cidades], caminho[quant_cidades];
	int visitado[quant_cidades], cont, mindistancia, proxVer, i, j;

	//caminho[] e distancias[] guarda os caminhos e as distãncias entre o vértice de origem e todos os outros vértices
    //cont grava a quantidade de vértices visitados até o momento

    /* Faz as trocas das posições da matriz de adjacência onde os vértices
    estão gravados com valo 0 para infinito, dessa forma aquela posição nunca será um caminho possível. Essa é uma maneira de identificar que não existe
    estrada entre as cidades que representam essa posição na matriz.*/
	for(i=0; i<quant_cidades; i++){
		for(j=0; j<quant_cidades; j++){
			if(G[i][j]==0){
				G[i][j]=INFINITO;
			}
		}
	}

	// Inicializa o caminho, as distancias e os vértices visitados
	for(i=0; i<quant_cidades; i++){
		distancias[i]=G[origem][i];
		caminho[i]=origem;
		visitado[i]=0;
	}

	distancias[origem]=0;
	visitado[origem]=1;
	cont=1;

	while(cont<quant_cidades-1){
		mindistancia=INFINITO;
		// Acha o véritice de menor distância
		for(i=0;i<quant_cidades;i++){
			if((distancias[i] < mindistancia) && (!visitado[i])){
				mindistancia=distancias[i];
				proxVer=i;
			}
		}
            // Verifica se existe um caminho melhor
			visitado[proxVer] = 1;
			for(i=0; i<quant_cidades; i++){
				if(!visitado[i]){
					if(mindistancia+G[proxVer][i] < distancias[i]){
						distancias[i] = mindistancia+G[proxVer][i];
						caminho[i] = proxVer;
					}
				}
			}
        cont++;
	}

    // Formata o print
    int x = 0;
    j = destino;
    int vetor[100];
    do{
        j = caminho[j];
        vetor[x] = j;
        x++;
    }while(j!=origem);
    x--;
    x--;

    printf("Menor percurso: %s ",cidades[origem].nome_cidade);
    for (i=x; i>=0; i--){
        printf("%s ", cidades[vetor[i]].nome_cidade);
    }
    printf("%s", cidades[destino].nome_cidade);

    printf("\n");
    printf("Distancia total: %d Km", distancias[destino]);
}

//Busca a posição da cidade na lista de cidades
int busca_posicao(char nome_cidade[], Cidade cidades[], int quant_cidades){
    for(int i=0; i<quant_cidades; i++){
        if(strcmp(nome_cidade, cidades[i].nome_cidade)==0){
            return i;
        }
    }
    return 0;
}

//Faz a matriz de adjacência
void FZ_Grafo(int **matriz, Arco arcos[], Cidade cidades[], int num_arestas, int quant_cidades){
    int aux1;
    int aux2;
    for (int i=0; i < num_arestas; i++){
        aux1 = busca_posicao(arcos[i].nome_cidade_origem, cidades, quant_cidades);
        aux2 = busca_posicao(arcos[i].nome_cidade_destino, cidades, quant_cidades);
        matriz[aux1][aux2] = arcos[i].distancia;
        matriz[aux2][aux1] = arcos[i].distancia;
    }
}

//Inicializa a matriz de adjacência com zeros
int **Aloca_matriz(int lin, int col){

    int **matriz = (int**)malloc(lin*sizeof(int*));

    for (int i=0; i<lin; i++){
        matriz[i] = (int*) malloc(col * sizeof(int));
        for (int j=0; j<col; j++){
            matriz[i][j] = 0;
        }
    }
    return matriz;
}

// Verifica se determinada cidade já foi adicionada na lista
int verifica_exixtencia(Cidade cidades[], char nome_cidade[], int quant_cidades){
    for(int i=0; i < quant_cidades; i++){
        if(strcmp(nome_cidade, cidades[i].nome_cidade) == 0){
            return 0;
        }
    }
    return 1;
}

// Aloca uma lista de cidades
void alocar_cidades(Cidade cidades[], Arco arcos[], int quant_arcos, int *quant_cidades){
    int aux = 2;
    int flag = 0;
    strcpy(cidades[0].nome_cidade, arcos[0].nome_cidade_origem);
    strcpy(cidades[1].nome_cidade, arcos[0].nome_cidade_destino);

    for(int i=1; i < quant_arcos; i++){
        flag = verifica_exixtencia(cidades, arcos[i].nome_cidade_origem, aux);
        if(flag == 1){
            strcpy(cidades[aux].nome_cidade, arcos[i].nome_cidade_origem);
            aux++;
        }
        flag = verifica_exixtencia(cidades, arcos[i].nome_cidade_destino, aux);
        if(flag == 1){
            strcpy(cidades[aux].nome_cidade, arcos[i].nome_cidade_destino);
            aux++;
        }
    }
    *quant_cidades = aux;
}

// Faz a leitura do arquivo
void carregar_arquivo(char cidade_origem[], char cidade_destino[], Arco arcos[], char nome_arquivo[], int *num_estradas){
    FILE *f = fopen(nome_arquivo, "r");
    char cidade1[20], cidade2[20];
    int num_linhas = 0;
    char c;

    // Conta quantas linhas o arquivo possui
    while(!feof(f)){
        c = getc(f);
        if(c == '\n'){
            num_linhas++;
        }
    }
    rewind(f);

    // Grava os dados do arquivo
    fscanf(f, "%d", num_estradas);
    for(int i=0; i<=num_linhas; i++){
        if(i == (num_linhas-2)){
            fscanf(f, " %s", cidade_origem);
            fscanf(f, " %s", cidade_destino);
        }
        else{
            fscanf(f, " %s", cidade1);
            strcpy(arcos[i].nome_cidade_origem, cidade1);
            fscanf(f, " %s", cidade2);
            strcpy(arcos[i].nome_cidade_destino, cidade2);
            fscanf(f, "%d", &arcos[i].distancia);
        }
    }
    fclose(f);
}


int main(){
    char nome_arquivo[100];
    int quant_cidades;

    printf("Digite o nome do arquivo de entrada: ");
    scanf("%s", nome_arquivo);

    char cidade_origem[20], cidade_destino[20]; // As cidades que desejo saber o menor caminho
    int num_estradas;
    Arco arcos[MAX]; // O arco é formado por 2 vértices e uma aresta

    carregar_arquivo(cidade_origem, cidade_destino, arcos, nome_arquivo, &num_estradas);

    // Para verificar se o programa está lendo o arquivo corretamente
    /*printf("Numero de estradas: %d\n\n", num_estradas);

    printf("Arcos:\n");
    for(int i=0; i < num_estradas; i++){
        printf("%s %s %d\n", arcos[i].nome_cidade_origem, arcos[i].nome_cidade_destino, arcos[i].distancia);
    }

    printf("\nCidade origem: %s\n", cidade_origem);
    printf("Cidade destino: %s\n", cidade_destino);*/

    Cidade *cidades;
    cidades = (Cidade *) malloc(MAX*sizeof(Cidade));
    alocar_cidades(cidades, arcos, num_estradas, &quant_cidades);
    int **matriz = Aloca_matriz(quant_cidades, quant_cidades);
    FZ_Grafo(matriz, arcos, cidades, num_estradas, quant_cidades);
    
  
    
    int origem = busca_posicao(cidade_origem, cidades, quant_cidades);
    int destino = busca_posicao(cidade_destino, cidades, quant_cidades);

    //Para verificar se a matriz de adjacência está correta
    
    for(int i=0; i<quant_cidades; i++){
        for(int j=0; j<quant_cidades; j++){
            printf("%3d ", matriz[i][j]);
        }
        printf("\n");
    }

    //Para verificar se a matriz de adjacência está correta
    /*
    printf("Lista de cidades: ");
    for(int i=0; i<quant_cidades; i++){
        printf("%s ", cidades[i].nome_cidade);
    }
    printf("\n");*/

    djstrika(matriz, quant_cidades, origem, destino, cidades);
    printf("\n");
    return 0;
}
