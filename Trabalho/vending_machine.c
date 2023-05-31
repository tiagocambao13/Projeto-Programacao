#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 48


// fds
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

void adicionar_produto(PRODUTO produtos[MAX], int* num_produto){
	PRODUTO produto;
	printf("Digita o código do produto que quer adicionar (Ex.:A0)");
	scanf("%[^\n]s",produto.codigo);
	getchar();

	printf("Digita o nome do produto:");
	scanf("%[^\n]s",produto.nome);
	getchar();

	printf("Digita o tipo do produto:");
	scanf("%[^\n]s",produto.tipo_produto);
	getchar();
	
	printf("Digita informações do produto:");
	scanf("%[^\n]s",produto.info_produto.informacao);
	getchar();
	
	printf("Digita as calorias do produto:");
	scanf("%f",&produto.info_produto.calorias);
	getchar();
	
	printf("Digita a validade do produto (dd/mm/aaaa):");
	scanf("%d/%d/%d",&produto.data_validade.dia,&produto.data_validade.mes,&produto.data_validade.ano);
	getchar();
	
	printf("Digita o preço do produto:");
	scanf("%f",&produto.price);
	getchar();
	printf("Digita a quantidade:");
	scanf("%d",&produto.quantidade);

	produtos[*num_produto]=produto;
	(*num_produto)++;	
}

void listar_produtos(PRODUTO produtos[MAX], int* num_produto){
	int i;

	for (i=0;i<*num_produto;i++){
		printf("\nCódigo:%s Nome:%s Tipo:%s Informação:%s Calorias:%.2f Data de validades:%d/%d/%d Preço:%.2f Quantidade:%d\n",produtos[i].codigo, 
	produtos[i].nome, produtos[i].tipo_produto, produtos[i].info_produto.informacao, produtos[i].info_produto.calorias, produtos[i].data_validade.dia,
	produtos[i].data_validade.mes,produtos[i].data_validade.ano,produtos[i].price,produtos[i].quantidade);
	}
	
}

void guardar_compra(PRODUTO produtos[MAX], int* num_produto, float total_dinheiro){
	FILE *txt, *adm;
	int i;

	txt = fopen("produtos.txt","w");
	if (txt == NULL){
		printf("Erro ao abrir o produtos.txt\n");
		return;
	}

	for (i=0; i<*num_produto; i++){
		fprintf(txt, "%s;%s;%s;%s;%f;%d/%d/%d;%f;%d\n", produtos[i].codigo, produtos[i].nome, produtos[i].tipo_produto,
				produtos[i].info_produto.informacao, produtos[i].info_produto.calorias, produtos[i].data_validade.dia,
				produtos[i].data_validade.mes, produtos[i].data_validade.ano, produtos[i].price, produtos[i].quantidade);
	}

	fclose(txt);

	adm = fopen("admin.txt","w");
	if (adm == NULL){
		printf("Erro ao abrir o admin.txt\n");
		return;
	} else {
		fprintf(adm, "%.2f", total_dinheiro);
	}

	fclose(adm);
}

float calculo_troco(PRODUTO produtos[MAX], int* num_produto, float preco_utilizador, int i, float* total_dinheiro){
	float troco;
	troco = preco_utilizador - produtos[i].price;

	if (preco_utilizador == produtos[i].price){
		printf("\nCompra realizada com sucesso\n");
	} else if (preco_utilizador > produtos[i].price){
		printf("\nCompra realizada com sucesso\nTroco: %.2f", troco);
	} else {
		while (preco_utilizador < produtos[i].price){
			float preco;
			printf("Restam %.2f para finalizar a compra ", produtos[i].price - preco_utilizador);
			scanf("%f", &preco);
			getchar();
			preco_utilizador += preco;
			if (preco_utilizador == produtos[i].price){
				printf("\nCompra realizada com sucesso\n");
			} else if (preco_utilizador > produtos[i].price){
				printf("\nCompra realizada com sucesso\nTroco: %.2f", troco);
			}
		}
	}

	produtos[i].quantidade--;
	*total_dinheiro += produtos[i].price;
	guardar_compra(produtos, num_produto, *total_dinheiro);

	return troco;
}


void comprar_produto(PRODUTO produtos[MAX], int* num_produto, char* codigo_utilizador,float* total_dinheiro){
	float preco_utilizador;
	int i;
			
	for (i=0;i<*num_produto;i++){
		if (strcmp(codigo_utilizador,produtos[i].codigo)== 0){
				if(produtos[i].quantidade!=0){
				printf("\nProduto encontrado:\n");
				printf("Insira %.2f para finalizar compra: ",produtos[i].price);
				scanf("%f",&preco_utilizador);
				getchar();
				calculo_troco(produtos, num_produto, preco_utilizador, i, &total_dinheiro);
			}else{
				printf("\nPorduto esgotado!\n");
			}
		}else {
			printf("Produto não encontrado!\n");
		}
	}
}



void carregar_produtos(PRODUTO produtos[MAX], int* num_produto){
	FILE *txt;
	char linha[200];
	
	txt = fopen("produtos.txt","r");
	
	if (txt==NULL){
		printf("Erro na arbertura do arquivo!\n");
		exit(1);
	}
	
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

float carregar_dinheiro(){
	FILE *adm;
	char linhas[5];
	float total_dinheiro;
	
	adm = fopen("admin.txt","r");
	
	if (adm==NULL){
		printf("Erro na abertura do arquivo!\n");
	} else {
		while(fgets(linhas, sizeof(linhas), adm) != NULL){
			sscanf(linhas, "%f", &total_dinheiro);
			//printf("%.2f\n", *total_dinheiro);
		}
	}
	
	fclose(adm);
	
	return total_dinheiro;
}



void guardar_produto(PRODUTO produtos[MAX],int* num_produto){
	PRODUTO produto;
	FILE *txt;
	int i;
	
	txt = fopen("produtos.txt","w");
	
	if (txt == NULL){
		printf("Erro ao abri o produtos.txt\n");
		return;
	}
	
	for (i=0;i<*num_produto;i++){
	printf("%s",produtos[i].codigo);
	fprintf(txt, "%s;%s;%s;%s;%f;%d/%d/%d;%f;%d\n", produtos[i].codigo, produtos[i].nome, produtos[i].tipo_produto,
	produtos[i].info_produto.informacao, produtos[i].info_produto.calorias, produtos[i].data_validade.dia,
	produtos[i].data_validade.mes,produtos[i].data_validade.ano,produtos[i].price,produtos[i].quantidade);
	}
	
	fclose(txt);
	
	printf("\nInformações guardadas com sucesso\n");
}


	
float dinheiro_total(PRODUTO produtos[MAX], int* num_produto) {
    FILE *adm;
    char opcao[1];
    float total_dinheiro;

    adm = fopen("admin.txt", "r");
    if (adm == NULL) {
        printf("Erro na abertura do arquivo!\n");
        return 0;
    } else {
        fscanf(adm, "%f", &total_dinheiro);
        fclose(adm);
    }

    printf("Dinheiro guardado: %.2f\n", total_dinheiro);
    printf("Quer levantar? (s/n): ");
    scanf("%s", opcao);
    getchar();

    if (strcmp(opcao, "s") == 0) {
        adm = fopen("admin.txt", "w");
        total_dinheiro = 0;
        fprintf(adm, "%.2f", total_dinheiro);
        fclose(adm);
        printf("Dinheiro levantado com sucesso.\n");
    } else {
        return total_dinheiro;
    }
}

void utilizador_admin(PRODUTO produtos[MAX], int* num_produto, char* codigo_utilizador){
	
	PRODUTO produto;
	
	if (strcmp(codigo_utilizador,"00")==0){
		admin(produtos,num_produto);
	} else{
		comprar_produto(produtos,num_produto,codigo_utilizador, dinheiro_total);
	}
}

void admin(PRODUTO produtos[MAX], int* num_produto, float* preco_maquina, float* total_dinheiro){
	int opcao;

	do {
		printf("\n*********Menu Admin***********");
		printf("\nEscolha a opção:\n");
		printf("0. Sair\n");
		printf("1. Adicionar produto\n");
		printf("2. Listar produtos\n");
		printf("3. Mostrar dinheiro guardado\n");
		printf("4. Voltar\n");
		printf("Opção: ");
		scanf("%d", &opcao);
		getchar();

		switch (opcao) {
			case 0:
				if (preco_maquina != NULL){
					*preco_maquina = *total_dinheiro;
				}
				exit(0);
			case 1:
				adicionar_produto(produtos, num_produto);
				guardar_produto(produtos, num_produto);
				break;
			case 2:
				listar_produtos(produtos, num_produto);
				break;
			case 3:
				*total_dinheiro = dinheiro_total(produtos, num_produto);
				return;
				break;
			case 4:
				return;
			default:
				printf("Opção inválida!\n");
				break;
		}
	} while (opcao != 0);
}

void main() {
    setlocale(LC_ALL, "Portuguese");
    PRODUTO produto[MAX];
    int num_produto = 0;
    char codigo_utilizador[3];
    float dinheiro_total=0;
    
	carregar_produtos(produto, &num_produto);
	dinheiro_total = carregar_dinheiro();
    
    do {
        printf("Lista de produtos:\n");
        listar_produtos(produto, &num_produto);
        
        printf("Digita o código: ");
        fgets(codigo_utilizador, sizeof(codigo_utilizador), stdin);
        codigo_utilizador[strcspn(codigo_utilizador, "\n")] = '\0';
        
        utilizador_admin(produto, &num_produto, codigo_utilizador);
    } while (1);  
}


