#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct linhas {
	int linha;
	struct linhas *prox;
};

typedef struct nodo {
	char* palavra;
	struct linhas *lista;
	struct nodo *prox;
} Verbete;

void imprime(Verbete *pi) {
	struct linhas *li;
	printf("Dicionário:\n");
	while (pi) {
		printf ("\n%s", pi->palavra);
		li = pi->lista;
		while(li) {
			printf (" - %d", li->linha);
			li = li->prox;
		}
		pi = pi->prox;
	}
	printf("\n");
}

void insere(struct linhas *lis, Verbete *indice[], int ini, Verbete *no, Verbete *ant, char *p, int linha) {
	int comparacao, flag = 0, cont, index;
	ant = NULL;
	no = ((indice[ini] == NULL) ? indice[0] : indice[ini]);
	Verbete *inicio = no;

	index = (((ini+1) < 26) ? (ini+1) : 25);
	
	while(no != indice[index]) {
		comparacao = strcmp(p, no->palavra);
		if(comparacao > 0) {
			ant = no;
		} else if(comparacao == 0){
			flag = 1;
			break;
		} else break;
		no = no->prox;
	}

	if(!flag) {
		Verbete *elemento;
		elemento = (Verbete *) malloc (sizeof(*elemento));
		elemento->lista = (struct linhas *) malloc (sizeof(*lis));
		elemento->lista->linha = linha;
		elemento->lista->prox = NULL;
		cont = strlen(p) + 1;
		elemento->palavra = (char*) malloc (cont);
		strcpy(elemento->palavra, p);
		if(ant == NULL){
			indice[ini] = elemento;
			elemento->prox = inicio->prox;
			inicio->prox = elemento;
		} else {
			elemento->prox = ant->prox;
			ant->prox = elemento;
		}
	} else {
		lis = no->lista;
		while(lis->prox != NULL) lis = lis->prox;
		struct linhas *proximo;
		proximo = (struct linhas *) malloc (sizeof(*proximo));
		proximo->linha = linha;
		lis->prox = proximo;
	}
}

int buscabin(char* vet[], int ini, int fim, char *num){
	if(ini > fim){
		return -1;
	} else {
		int meio = (ini+fim)/2;
		int posicao = strcmp(vet[meio], num);
		if(posicao == 0){
      	return meio;
		} else if(posicao > 0){
			fim = meio-1;
			return buscabin(vet, ini, fim, num);
		} else {
			ini = meio+1;
			return buscabin(vet, ini, fim, num);
		}
	}
}

void minuscula(char *palavras) {
	int j = 0;
	while (palavras[j]){
		palavras[j] = (palavras[j] > 64 && palavras[j] < 91) ? palavras[j]+32 : palavras[j];
		j++;
	}
}

int main(int argc, char *argv[]) {
	int l = 0, i = 0, j, k, m, flag = 0;
	FILE *arq, *words;	
	char *stopwords[183];
	
	struct linhas *lista;
	Verbete *raiz, *laco, *anterior;
	raiz = (Verbete*) malloc (sizeof(*raiz));
	raiz->palavra = "";
	raiz->prox = NULL;
	
	Verbete *indice[26];
	for(m = 0; m < 26; m++) indice[m] = NULL;
	indice[0] = raiz;
	
	//os dois primeiros argumentos são ./q3 e 4, depois vem stopwords.txt e livrocompleto.txt
	if ((words = fopen(argv[2], "r")) && (arq = fopen(argv[3], "r"))) {
		fseek(words, 0, SEEK_SET);
		while(!feof(words)) {
			char pal[20];
			if (fscanf(words, "%s", pal) == 1){
				int cont = strlen(pal) + 1;
				stopwords[i] = (char*) malloc (cont);
				strcpy(stopwords[i], pal);
				i++;
			}
		}
		fclose(words);
		
		fseek(arq, 0, SEEK_SET);
		while(!feof(arq)) {
			char linha[150], *palavras;
			if (fgets (linha, 150, arq) != NULL){
				l++;

				palavras = strtok(linha, " ,-.\"\'’*_/;!:?1234567890()[]\n");
				while (palavras != NULL) {
					minuscula(palavras);
					flag = buscabin(stopwords, 0, 179, palavras);

					if(flag == -1) {
						int letra = palavras[0] % 97;
						insere(lista, indice, letra, laco, anterior, palavras, l);
					}
					flag = 0;
					palavras = strtok(NULL, " ,-.\"\'’*_/;!:?1234567890()[]\n");
				}
			}
		}
		fclose(arq);
		imprime(raiz->prox);
	} else printf ("Erro na abertura dos arquivos\n");
	return 0;
}
