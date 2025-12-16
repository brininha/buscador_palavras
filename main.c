#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// struct para guardar todas as linhas de ocorrencia de uma palavras
typedef struct NoLinha {
    int numeroLinha;      
    struct NoLinha *prox; 
} NoLinha;

// struct para guardar uma palavra e a qtd de ocorrencias dela
typedef struct {
    char *palavra;        
    int ocorrencias;     
    NoLinha *listaLinhas; 
} ItemIndice;

// struct para indexar todas as palavras identificadas no texto (com lista)
typedef struct {
    ItemIndice *itens; 
    int qtd;           
    int capacidade;    
} ListaSequencial;

// struct para indexar todas as palavras identificadas no texto (com arvore)
typedef struct NoArvore {
    char *palavra;
    int ocorrencias;
    NoLinha *listaLinhas;
    struct NoArvore *esq; 
    struct NoArvore *dir; 
} NoArvore;

// struct para guardar as linhas do texto do arquivo txt
typedef struct {
    char **linhas;   
    int totalLinhas; 
    int capacidade;  
} TextoMemoria;

// essas sao as funcoes auxiliares
void adicionar_linha(NoLinha **raiz, int linha);
void carregar_arquivo(char *nomeArquivo, TextoMemoria *texto, void *indice, char *tipoIndice, int *comp);

// funcoes para o caso de o usuario escolher o tipo lista
void inicializar_lista(ListaSequencial *lista);
void inserir_lista(ListaSequencial *lista, char *palavra, int linha, int *comp);
ItemIndice* buscar_lista(ListaSequencial *lista, char *palavra, int *comp);
void liberar_lista(ListaSequencial *lista);
int contar_palavras_unicas_lista(ListaSequencial *lista);

// funcoes para o caso de o usuario escolher o tipo arvore 
void inserir_arvore(NoArvore **raiz, char *palavra, int linha, int *comp);
NoArvore* buscar_arvore(NoArvore *raiz, char *palavra, int *comp);
int altura_arvore(NoArvore *raiz);
void liberar_arvore(NoArvore *raiz);
int contar_palavras_unicas_arvore(NoArvore *raiz);

// obs: as funcoes estao implementadas no final do arquivo

int main(int argc, char *argv[]) {

    // primeiro estamos pegando os argumentos passados no main, validando e guardando em variaveis
    if (argc != 3) {
        printf("Uso: %s <arquivo> <tipo_indice>\n", argv[0]);
        return 1;
    }

    char *nomeArquivo = argv[1];
    char *tipoIndice = argv[2];
    
    if (strcmp(tipoIndice, "lista") != 0 && strcmp(tipoIndice, "arvore") != 0) {
        printf("O tipo de indice deve ser 'lista' ou 'arvore'.\n");
        return 1;
    }

    // inicializando struct de texto
    TextoMemoria texto;
    texto.linhas = malloc(10 * sizeof(char*)); 
    texto.totalLinhas = 0;
    texto.capacidade = 10;
    
    ListaSequencial indiceLista; 
    NoArvore *indiceArvore = NULL;
    
    if (strcmp(tipoIndice, "lista") == 0) {
        inicializar_lista(&indiceLista);
    }

    // essas comparacoes serao importantes para medirmos resultados no relatorio
    int comparacoesConstrucao = 0; 

    void *ptrIndice = (strcmp(tipoIndice, "lista") == 0) ? (void*)&indiceLista : (void*)&indiceArvore;
    carregar_arquivo(nomeArquivo, &texto, ptrIndice, tipoIndice, &comparacoesConstrucao);

    // imprimindo infos do arquivo
    printf("Arquivo: '%s'\n", nomeArquivo);
    printf("Tipo de indice: '%s'\n", tipoIndice);
    printf("Numero de linhas no arquivo: %d\n", texto.totalLinhas);
    
    int totalPalavras = 0;
    if (strcmp(tipoIndice, "lista") == 0) 
        totalPalavras = contar_palavras_unicas_lista(&indiceLista);
    else 
        totalPalavras = contar_palavras_unicas_arvore(indiceArvore);

    printf("Total de palavras unicas indexadas: %d\n", totalPalavras);

    if (strcmp(tipoIndice, "arvore") == 0) {
        printf("Altura da arvore: %d\n", altura_arvore(indiceArvore));
    }
    
    printf("Numero de comparacoes realizadas para a construcao do indice: %d\n", comparacoesConstrucao);

    // aqui comeca a logica de busca
    char comando[100];
    char palavraBusca[100];
    
    while (1) {
        printf("> ");
        scanf("%s", comando); 

        if (strcmp(comando, "fim") == 0) {
            break;
        } 
        else if (strcmp(comando, "busca") == 0) {
            scanf("%s", palavraBusca);
            // deixando a palavra em minusculas pra nao ter erro
            for(int i = 0; palavraBusca[i]; i++) {
                palavraBusca[i] = tolower(palavraBusca[i]);
            }

            int compBusca = 0; 
            NoLinha *resultadoLinhas = NULL;
            int totalOcorrencias = 0;

            // checando tipo de busca e buscando
            if (strcmp(tipoIndice, "lista") == 0) {
                ItemIndice *res = buscar_lista(&indiceLista, palavraBusca, &compBusca);
                if (res) {
                    resultadoLinhas = res->listaLinhas;
                    totalOcorrencias = res->ocorrencias;
                }
            } else {
                NoArvore *res = buscar_arvore(indiceArvore, palavraBusca, &compBusca);
                if (res) {
                    resultadoLinhas = res->listaLinhas;
                    totalOcorrencias = res->ocorrencias;
                }
            }
            // imprimindo resultados obtidos
            if (resultadoLinhas) {
                printf("Existem %d ocorrências da palavra '%s' na(s) seguinte(s) linha(s):\n", totalOcorrencias, palavraBusca);
                NoLinha *atual = resultadoLinhas;
                while (atual != NULL) {
                    printf("%05d: %s", atual->numeroLinha, texto.linhas[atual->numeroLinha - 1]); 
                    atual = atual->prox;
                }
            } else {
                printf("Palavra '%s' nao encontrada.\n", palavraBusca);
            }
            printf("Numero de comparacoes: %d\n", compBusca);

        } else {
            char lixo[100]; fgets(lixo, 100, stdin);
            printf("Opcao invalida!\n");
        }
    }
    // liberando memoria da estrutura escolhida
    if (strcmp(tipoIndice, "lista") == 0) {
        liberar_lista(&indiceLista);
    } else {
        liberar_arvore(indiceArvore);
    }
    
    for (int i = 0; i < texto.totalLinhas; i++) {
        free(texto.linhas[i]);
    }
    free(texto.linhas);

    return 0;
}

void adicionar_linha(NoLinha **raiz, int linha) {
    
    if (*raiz == NULL) {
        NoLinha *novoNo = (NoLinha *) malloc(sizeof(NoLinha));
        novoNo->numeroLinha = linha;
        novoNo->prox = NULL;
        *raiz = novoNo;
        return;
    }

    NoLinha *aux = *raiz;
    
    while (aux->prox != NULL) {
    
        if (aux->numeroLinha == linha) return;
        aux = aux->prox;
    }
    if (aux->numeroLinha == linha) return;

    NoLinha *novoNo = (NoLinha *) malloc(sizeof(NoLinha));
    novoNo->numeroLinha = linha;
    novoNo->prox = NULL;
    aux->prox = novoNo;
}

void carregar_arquivo(char *nomeArquivo, TextoMemoria *texto, void *indice, char *tipoIndice, int *comp) {
    FILE *pont_arq = fopen(nomeArquivo, "r");
    if (pont_arq == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1); 
    }

    char buffer[2000]; 

    while (fgets(buffer, sizeof(buffer), pont_arq) != NULL) {
        
        if (texto->totalLinhas >= texto->capacidade) {
             texto->capacidade *= 2;
             texto->linhas = realloc(texto->linhas, texto->capacidade * sizeof(char*));
        }
        texto->linhas[texto->totalLinhas] = strdup(buffer);
        int linhaAtual = texto->totalLinhas + 1;
        texto->totalLinhas++;

        char palavraTemp[200];
        int p = 0;
        
        for (int i = 0; buffer[i] != '\0'; i++) {
            char c = buffer[i];
            
            if (isalnum(c)) {
                palavraTemp[p++] = tolower(c); 
            } 
            else if (p > 0) {
                palavraTemp[p] = '\0'; 
                if (strcmp(tipoIndice, "lista") == 0) {
                    inserir_lista((ListaSequencial*)indice, palavraTemp, linhaAtual, comp);
                } else {
                    inserir_arvore((NoArvore**)indice, palavraTemp, linhaAtual, comp);
                }
                p = 0; 
            }
        }
        
        if (p > 0) {
            palavraTemp[p] = '\0';
            if (strcmp(tipoIndice, "lista") == 0) {
                inserir_lista((ListaSequencial*)indice, palavraTemp, linhaAtual, comp);
            } else {
                inserir_arvore((NoArvore**)indice, palavraTemp, linhaAtual, comp);
            }
        }
    }
    fclose(pont_arq);
}

void inicializar_lista(ListaSequencial *lista) {
    lista->qtd = 0;
    lista->capacidade = 50; 
    lista->itens = (ItemIndice *) malloc(lista->capacidade * sizeof(ItemIndice));
}

void inserir_lista(ListaSequencial *lista, char *palavra, int linha, int *comp) {

    for (int i = 0; i < lista->qtd; i++) {
        (*comp)++; 

        if (strcmp(lista->itens[i].palavra, palavra) == 0) {
            lista->itens[i].ocorrencias++; 
            adicionar_linha(&(lista->itens[i].listaLinhas), linha); 
            return;
        }
    }

    if (lista->qtd == lista->capacidade) {
        lista->capacidade *= 2; 
        ItemIndice *temp = realloc(lista->itens, lista->capacidade * sizeof(ItemIndice));
        if (temp == NULL) {
            printf("Erro de memoria!\n");
            exit(1);
        }
        lista->itens = temp;
    }

    lista->itens[lista->qtd].palavra = strdup(palavra); 
    lista->itens[lista->qtd].ocorrencias = 1;
    lista->itens[lista->qtd].listaLinhas = NULL;

    adicionar_linha(&(lista->itens[lista->qtd].listaLinhas), linha);

    lista->qtd++;
}
ItemIndice* buscar_lista(ListaSequencial *lista, char *palavra, int *comp) {
    for (int i = 0; i < lista->qtd; i++){
        (*comp)++;
        
        if (strcmp(lista->itens[i].palavra, palavra) == 0){
            return &lista->itens[i];
        }
    }
    return NULL;
}
int contar_palavras_unicas_lista(ListaSequencial *lista) {
    return lista->qtd; 
}

void liberar_lista(ListaSequencial *lista) {
    for (int i = 0; i < lista->qtd; i++) {
        free(lista->itens[i].palavra);
        
        NoLinha *atual = lista->itens[i].listaLinhas;
        while (atual != NULL) {
            NoLinha *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(lista->itens);
}

void inserir_arvore(NoArvore **raiz, char *palavra, int linha, int *comp) {
    if (*raiz == NULL) {
        *raiz = (NoArvore*) malloc(sizeof(NoArvore));
        (*raiz)->palavra = strdup(palavra);
        (*raiz)->ocorrencias = 1;
        (*raiz)->dir = NULL;
        (*raiz)->esq = NULL;
        (*raiz)->listaLinhas = NULL;
        adicionar_linha(&((*raiz)->listaLinhas), linha);
        return;
    }

    (*comp)++;

    if (strcmp(palavra, (*raiz)->palavra) == 0) {
        (*raiz)->ocorrencias++;
        adicionar_linha(&((*raiz)->listaLinhas), linha);
    } else if (strcmp(palavra, (*raiz)->palavra) < 0) {
        inserir_arvore(&((*raiz)->esq), palavra, linha, comp);
    } else if (strcmp(palavra, (*raiz)->palavra) > 0) {
        inserir_arvore(&((*raiz)->dir), palavra, linha, comp);
    }
}

NoArvore* buscar_arvore(NoArvore *raiz, char *palavra, int *comp) {
    if (raiz == NULL)
    {
        return NULL;
    }
    (*comp)++;
    if (strcmp(palavra, raiz->palavra) == 0)
    {
        return raiz;
    }
    else if (strcmp(palavra, raiz->palavra) < 0)
    {
        return buscar_arvore(raiz->esq, palavra, comp);
    }
    else 
    {
        return buscar_arvore(raiz->dir, palavra, comp);
    }
}

int altura_arvore(NoArvore *raiz) {
    if (raiz == NULL) {
        return -1;
    }

    int h_esq = altura_arvore(raiz->esq);
    int h_dir = altura_arvore(raiz->dir);

    if (h_esq > h_dir) {
        return 1 + h_esq;
    } else {
        return 1 + h_dir;
    }
}

int contar_palavras_unicas_arvore(NoArvore *raiz) {
    if (raiz == NULL)
    {
        return 0;
    }
    int resultadoEsq = contar_palavras_unicas_arvore(raiz->esq);
    int resultadoDir = contar_palavras_unicas_arvore(raiz->dir);

    return 1 + resultadoEsq + resultadoDir;
}

void liberar_arvore(NoArvore *raiz) {
    if (raiz == NULL) {
        return;
    }
    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);

    free(raiz->palavra);

    NoLinha *pontAux = raiz->listaLinhas;
    while (pontAux != NULL) {
        NoLinha *temp = pontAux->prox;
        free(pontAux);
        pontAux = temp;
    }
    free(raiz);
}