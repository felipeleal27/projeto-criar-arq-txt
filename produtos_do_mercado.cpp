#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale.h>

struct produto
{
	char produto[20];
	char codigo[45];
	char area[30];
	char status;
	
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void excluir(FILE *);
void criararq(FILE *);


int main()
{
	setlocale(LC_ALL,"");
	int op;
	
	FILE *arq;
	if ((arq = fopen("produtos.dat", "rb+")) == NULL)
	{
		if ((arq = fopen("produtos.dat", "wb+")) == NULL)
		{
			printf("Falha ao abrir o arquivo!\n");
			system("pause");
		}
	}
	
	do
	{
		system("cls");
		printf("=============CADASTRAR=============\n");
		printf(" 1. Cadastrar novo produto.\n");
		printf(" 2. Consultar produto cadastrado.\n");
		printf(" 3. Excluir produto.\n");
		printf(" 4. Criar arquivo.\n");
		printf(" 5. Sair do programa.\n\n");
		
		printf("Quantidade de produtos: %d\n", tamanho(arq));
		printf("==================================\n");
		printf("Opção: ");
		scanf("%d",&op);
		
	switch (op)
	{
		case 1:
			cadastrar(arq);
			break;
		case 2:
			consultar(arq);
			break;
		case 3:
			excluir(arq);
			break;
		case 4:
			criararq(arq);
			break;
	}	
	
	} while (op != 5);
}

void cadastrar(FILE *arq)
{
	 produto cadastrar;
	 char verdadeiro;
	 char confirma;
	 
	 cadastrar.status = ' ';
	 fflush(stdin);
	 
	 printf("Cadastrando produto...\n");
	 printf("Produto número: %d\n", tamanho(arq) + 1);
	 
	 printf("Nome do Produto.....: ");
	 gets(cadastrar.produto);
	 printf("Código de Barras....: ");
	 gets(cadastrar.codigo);
	 printf("Local do produto....: ");
	 gets(cadastrar.area);
	 
	 printf("\nConfirmar cadastro<s/n>: ");
	 fflush(stdin);
	 scanf("%c", &confirma);
	 
	 if(toupper(confirma) == 'S')
	 {
	 	printf("\ngravando...\n\n");
	 	fseek(arq, 0, SEEK_END);
	 	fwrite(&cadastrar, sizeof(produto), 1, arq);
	 }
}

void consultar(FILE *arq)
{
	produto consulta;
	int nr;
	printf("Consulta pelo códio\n");
	printf("Informe o código: ");
	scanf("%d", &nr);
	
	if( (nr <= tamanho(arq)) && (nr>0) )
	{
		fseek(arq, (nr - 1) * sizeof(produto), SEEK_SET);
		fread(&consulta, sizeof(produto), 1, arq);
		if(consulta.status == ' ')
		{
			printf("Produto.............:%s\n", consulta.produto);
			printf("Código de barras....:%s\n", consulta.codigo);
			printf("Local do produto....:%s\n", consulta.area);
		}
		
		else
		{
			printf("Registro excluído!");
		}
	}
	
	else
	{
		printf("Número do registro inválido.");
	}
	system("pause");
}

void excluir(FILE *arq)
{
	produto cadastro;
	char confirma;
	int nr;

	printf("\nInforme o codigo do registro para excluir\n");
	scanf("%d", &nr);
	if ((nr <= tamanho(arq)) && (nr>0))
	{
		fseek(arq, (nr - 1) * sizeof(produto), SEEK_SET);
		fread(&cadastro, sizeof(produto), 1, arq);
		if (cadastro.status == ' ')
		{
			printf("\nProduto.............:%s", cadastro.produto);
			printf("\nCód de Barras.......:%s", cadastro.codigo);
			printf("\nLocal do produto....:%s\n", cadastro.area);
			printf("\nConfirma a exclusao: <s/n>\n");
			getchar();
			scanf("%c", &confirma);

			if (toupper(confirma) == 'S')
			{
				printf("\nexcluindo...\n\n");
				fseek(arq, (nr - 1) * sizeof(produto), SEEK_SET);
				cadastro.status = '*';
				fwrite(&cadastro, sizeof(produto), 1, arq);
			}
		}
	else
		printf("Registro inexistente! \n");
	}
	else
	{
		printf("\nNumero de registro invalido!\n");
	}
	system("pause");
}

void criararq(FILE *arq)
{
	char nomearq[20];
	printf("Nome do arquivo texto:");
	scanf("%s", nomearq);
	strcat(nomearq, ".txt");
	FILE *arqtxt = fopen(nomearq, "w");
	if (!arqtxt)
	{
		printf("Nao foi possivel criar esse arquivo!\n");   // ====================
		system("pause");
		//return;
	}
	fprintf(arqtxt, "Produto                    Cód. Barras                    Local                    Status\n");
	fprintf(arqtxt, "==========================================================================================\n");
	int nr;
	produto cadastro;
	for (nr = 0; nr<tamanho(arq); nr++)
	{
		fseek(arq, nr * sizeof(produto), SEEK_SET);
		fread(&cadastro, sizeof(produto), 1, arq);
		fprintf(arqtxt, "%-27s%-31s%-27s- %c\n", cadastro.produto, cadastro.codigo, cadastro.area, cadastro.status);
	}
	fprintf(arqtxt, "=============================================================================================\n");
	fclose(arqtxt);
	
}

int tamanho(FILE *arq)
{
	fseek(arq, 0, SEEK_END);
	return ftell(arq) / sizeof(produto);
}
