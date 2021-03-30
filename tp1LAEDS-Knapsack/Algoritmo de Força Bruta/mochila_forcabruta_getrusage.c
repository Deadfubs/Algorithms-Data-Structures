/************************************************
LAED1 - Trabalho Pratico 1
Aluno: Fulvio Taroni Monteforte
       Thiago Lima Bahia Santos
Matricula: 20183005944
           20183000302
Descricao do programa: Problema da Mochila | Algoritmo de Forca Bruta + getrusage()
Data: 24/09/19
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <sys/resource.h>

#define MAX_TAM 100

typedef struct {
	int peso;
	int valor;
} Item;

void gerar_arquivo();
void carregar_arquivo(int *cap_mochila, int *num_item, Item mochila[]);
void imprimir_arquivo(int num_item, Item mochila[], int melhor_mochila[]);
void mochila_forcabruta(int cap_mochila, int num_item, Item mochila[], int melhor_mochila[]);

/* Teste da medicao do tempo*/
int main(int argc, char *argv[])
{
	// Variaveis relacionadas com a medicao do tempo:
	int who = RUSAGE_SELF; //man: information shall be returned about resources used by the current process
	struct rusage usage;

	long utotalmicroseg, utotalseg; //tempo usuario: tempo que a CPU gasta executando o programa
	long stotalmicroseg, stotalseg; //tempo sistema: tempo que a CPU gasta executando chamadas
									//de sistemas para o programa
	
	//coloque aqui o algoritmo

	int i, cap_mochila, num_item, melhor_mochila[MAX_TAM];
	Item mochila[MAX_TAM];

	//Função gerar_arquivo() suprimida do código. Utilizar, se necessário.
	//gerar_arquivo();
	carregar_arquivo(&cap_mochila, &num_item, mochila);	
	mochila_forcabruta(cap_mochila, num_item, mochila, melhor_mochila);
	imprimir_arquivo(num_item, mochila, melhor_mochila);
	
	getrusage(who, &usage);
	
	//tempo de usuário na CPU
	utotalseg = usage.ru_utime.tv_sec;			//segundos
	utotalmicroseg = usage.ru_utime.tv_usec;	//microsegundos

	//tempo de sistema na CPU
	stotalseg = usage.ru_stime.tv_sec;			//segundos
	stotalmicroseg = usage.ru_stime.tv_usec; 	//microsegundos

	printf ("\n");
	printf ("***************************************************************\n");
	printf ("Tempo de usuario: %ld segundos e %ld microssegundos.\n", utotalseg, utotalmicroseg);
	printf ("Tempo de sistema: %ld segundos e %ld microssegundos.\n", stotalseg, stotalmicroseg);
	printf ("***************************************************************\n");
	printf ("\n");
	
	return(0);
}

void gerar_arquivo() {

	FILE *ptr_arq;
	int i, num_item, cap_mochila, sum_peso;
	Item rand_mochila[MAX_TAM];

	printf("Numero de itens : ");
	scanf("%d", &num_item);

	sum_peso = 0;
	for (i=0; i<num_item; i++) {	
		rand_mochila[i].peso = ( rand() % 9 ) + 1;
		sum_peso = sum_peso + rand_mochila[i].peso;
	}

	for (i=0; i<num_item; i++)		
		rand_mochila[i].valor = ( rand() % 9 ) + 1;

	cap_mochila = ( rand() % (sum_peso / 2) ) + 1;

	ptr_arq = fopen("arquivo_leitura.txt","w");	

	if (!ptr_arq) {
	      printf("Erro na abertura do arquivo!\n");   
	      exit(1);
	}
	
	else {

		fprintf (ptr_arq, "%d\n", cap_mochila);
		fprintf(ptr_arq, "%d\n", num_item);

		for(i=0; i<num_item; i++)
			fprintf (ptr_arq, "%d %d\n", rand_mochila[i].peso, rand_mochila[i].valor);
				
	}

	fclose(ptr_arq);	

}

void carregar_arquivo(int *cap_mochila, int *num_item, Item mochila[]) {

	FILE *ptr_arq;
	int i;

	ptr_arq = fopen("arquivo_leitura.txt","r");

	if (!ptr_arq) {
	      printf("Erro na abertura do arquivo!\n");   
	      exit(1);
	}
	
	else {

		fscanf (ptr_arq, "%d", cap_mochila);		
		fscanf (ptr_arq, "%d", num_item);				

		i = 0;
		while (!feof(ptr_arq)) {
			fscanf (ptr_arq, "%d %d", &mochila[i].peso, &mochila[i].valor);
			i++;				
		}
	}

	fclose(ptr_arq);

}

void imprimir_arquivo(int num_item, Item mochila[], int melhor_mochila[]) {

	FILE *ptr_arq;
	int i, sum_peso, sum_valor;

	ptr_arq = fopen("arquivo_escrita.txt","w");

	if (!ptr_arq) {
	      printf("Erro na abertura do arquivo!\n");   
	      exit(1);
	}

	else {		

		sum_peso = 0;
		sum_valor = 0;

		for (i=0; i<num_item; i++)
			if (melhor_mochila[i] == 1) {
				fprintf (ptr_arq, "%d %d %d\n", i, mochila[i].peso, mochila[i].valor);
				sum_peso = sum_peso + mochila[i].peso;
				sum_valor = sum_valor + mochila[i].valor;
			}

		fprintf (ptr_arq, "%d %d\n", sum_peso, sum_valor);			
		
	}

	fclose(ptr_arq);		

}

void mochila_forcabruta(int cap_mochila, int num_item, Item mochila[], int melhor_mochila[]) {

	int j, k, temp_mochila[MAX_TAM];
	unsigned long long int i, iter;
	Item temp_item, melhor_item;

	iter = pow(2, num_item);

	for (i=0; i<num_item; i++)
		temp_mochila[i] = 0;

	for (i=0; i<iter; i++) {

		j = num_item-1;
		temp_item.peso = 0;
		temp_item.valor = 0;
		
		while (temp_mochila[j] != 0 && j >= 0) {
			temp_mochila[j] = 0;
			j--;
		}			

		temp_mochila[j] = 1;
		
		for (k=0; k<num_item; k++) {

			if (temp_mochila[k] == 1) {
				temp_item.peso = temp_item.peso + mochila[k].peso;
				temp_item.valor = temp_item.valor + mochila[k].valor;
			}

		}

		if (temp_item.valor > melhor_item.valor && temp_item.peso <= cap_mochila) {

			melhor_item.peso = temp_item.peso;
			melhor_item.valor = temp_item.valor;
			
			for (k=0; k<num_item; k++)
				melhor_mochila[k] = temp_mochila[k];

		}

	}

}
