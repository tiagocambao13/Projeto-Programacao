#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 48

typedef struct data{
	int dia,mes,ano;
}DATA;

typedef struct info{
	char informacao[100];
	float calorias;
}INFO;

typedef struct produtos{
	char codigo[3];
	char nome[20];
	char tipo_produto[20];
	INFO info_produto;
	DATA data_validade;
	float price;
	int quantidade;
}PRODUTO;

void admistrador(PRODUTO produtos[MAX], int* num_produto){
	PRODUTO produto;
	int opcao;
	
	do{
		printf("\nEscolha a opção:\n");
		printf("1.Adicionar produto\n");
		printf("2.Guardar alterações\n");
		printf("3.Listar produtos\n");
		printf("4.Comprar produto\n");
		printf("Opção:");
		scanf("%d",&opcao);
		getchar();
		switch(opcao){
			case 0:
				break;
			case 1:
				adicionar_produto(produto,&num_produto);
				break;
			case 2:
				guardar_produto(produto,&num_produto);
				break;
			case 3:
				listar_produtos(produto,&num_produto);
				break;
			case 4:
				comprar_produto(produto,&num_produto);
				break;
			default:
				printf("Opção inválida!");
				break;
		}	
	}while(opcao!=0);
}


void comprar_produto(PRODUTO produtos[MAX], int* num_produto, char* codigo_utilizador){
	PRODUTO produto;
	float preco_utilizador;
	int i;
	
	for (i=0;i<*num_produto;i++){
		if (strcmp(codigo_utilizador,"00")==0){
			admistrador(produto,&num_produto);
		}else if (strcmo(codigo_utilizador,"11")==0){
			exit;
			}else if (strcmp(codigo_utilizador,produtos[i].codigo)==0 && produtos[i].quantidade!=0){
					printf("Insira %.2f para finalizar compra",produtos[i].price);
					scanf("%f",&preco_utilizador);
				do{
					if (preco_utilizador<produtos[i].price){
						printf("Dinheiro insuficiente!");
					} else {
						printf("Compra realizada com sucesso");
						produtos[i].quantidade--;
					}
				}while(preco_utilizador!=produtos[i].price);
				break;
			} else {
				printf("Produto esgotado!\n");
			}
		}
	guardar_compra(produto,&num_produto);
}


void carregar_produtos(PRODUTO produtos[MAX], int* num_produto){
	FILE *txt;
	char linha[200];
	
	txt = fopen("produtos.txt","r");
	
	while(fgets(linha, sizeof (linha), txt) != NULL){
		PRODUTO produto;
		sscanf(linha, "%[^;];%[^;];%[^;];%[^;];%f;%d/%d/%d;%f;%d",
			produto.codigo, produto.nome, produto.tipo_produto, produto.info_produto.informacao, &produto.info_produto.calorias, 
			&produto.data_validade.dia,&produto.data_validade.mes,&produto.data_validade.ano, &produto.price, &produto.quantidade);	
		produtos[*num_produto] = produto;
    	(*num_produto)++;
	}
	
	fclose(txt);
}

void adicionar_produto(PRODUTO produtos[MAX], int* num_produto){
	PRODUTO produto;
	printf("Digita o código do produto que quer adicionar (Ex.:A0)");
	scanf("%s",produto.codigo);
	getchar();
	printf("Digita o nome do produto:");
	scanf("%s",produto.nome);
	getchar();
	printf("Digita o tipo do produto:");
	scanf("%s",produto.tipo_produto);
	getchar();
	printf("Digita informações do produto:");
	fgets(produto.info_produto.informacao,200,stdin);
	getchar();
	printf("Digita as calorias do produto:");
	scanf("%f",&produto.info_produto.calorias);
	printf("Digita a validade do produto (dd/mm/aaaa):");
	scanf("%d/%d/%d",&produto.data_validade.dia,&produto.data_validade.mes,&produto.data_validade.ano);
	printf("Digita o preço do produto:");
	scanf("%f",&produto.price);
	printf("Digita a quantidade:");
	scanf("%d",&produto.quantidade);

	produtos[*num_produto]=produto;
	(*num_produto)++;	

}

void listar_produtos(PRODUTO produtos[MAX], int* num_produto){
	int i;
	
	for (i=0;i<*num_produto;i++){
		if (strlen(produtos[i].codigo) == 0) continue;
		printf("Código:%s Nome:%s Tipo:%s Informação:%s Calorias:%.2f Data de validades:%d/%d/%d Preço:%.2f Quantidade:%d\n",produtos[i].codigo, 
	produtos[i].nome, produtos[i].tipo_produto, produtos[i].info_produto.informacao, produtos[i].info_produto.calorias, produtos[i].data_validade.dia,
	produtos[i].data_validade.mes,produtos[i].data_validade.ano,produtos[i].price,produtos[i].quantidade);
	}
	
}

void guardar_produto(PRODUTO produtos[MAX],int* num_produto){
	FILE *txt;
	int i;
	
	txt = fopen("produtos.txt","w");
	
	if (txt == NULL){
		printf("Erro ao abri o produtos.txt\n");
		return;
	}
	
	for (i=0;i<*num_produto;i++){
	fprintf(txt, "%s;%s;%s;%s;%f;%d/%d/%d;%f;%d\n", produtos[i].codigo, produtos[i].nome, produtos[i].tipo_produto,
	produtos[i].info_produto.informacao, produtos[i].info_produto.calorias, produtos[i].data_validade.dia,
	produtos[i].data_validade.mes,produtos[i].data_validade.ano,produtos[i].price,produtos[i].quantidade);
	}
	
	fclose(txt);
	
	printf("Informações guardadas com sucesso\n");
}


void guardar_compra(PRODUTO produtos[MAX],int* num_produto){
	FILE *txt;
	int i;
	
	txt = fopen("produtos.txt","w");
	
	if (txt == NULL){
		printf("Erro ao abri o produtos.txt\n");
		return;
	}
	
	for (i=0;i<*num_produto;i++){
	fprintf(txt, "%s;%s;%s;%s;%f;%d/%d/%d;%f;%d\n", produtos[i].codigo, produtos[i].nome, produtos[i].tipo_produto,
	produtos[i].info_produto.informacao, produtos[i].info_produto.calorias, produtos[i].data_validade.dia,
	produtos[i].data_validade.mes,produtos[i].data_validade.ano,produtos[i].price,produtos[i].quantidade);
	}
	
	fclose(txt);
}

main(){
	setlocale(LC_ALL, "Portuguese");
	PRODUTO produto[MAX];
	int num_produto=0;
	char codigo_utilizador[3];
	FILE *txt;
	
	txt = fopen("produtos.txt","r");
	
	carregar_produtos(produto,&num_produto);
	
	do{
		listar_produtos(produto,&num_produto);
		printf("Digita o código: ");
		fgets(codigo_utilizador,3,stdin);
		comprar_produto(produto,&num_produto);
	}while(strcmp());
}

