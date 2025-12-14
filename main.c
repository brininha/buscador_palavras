#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// definicao das estruturas de dados que vamos usar

// estrutura para armazenar as linhas onde a palavra ocorre (lista ligada simples)
typedef struct NoLinha {
    int numeroLinha;
    struct NoLinha *prox;
} NoLinha;

// estrutura para o indice (lista ligada)
typedef struct NoLista {
    char *palavra;
    int ocorrencias;
    NoLinha *listaLinhas; // ponteiro para a lista de linhas acima
    struct NoLista *prox;
} NoLista;

// estrutura para o indice (arvore binaria de busca)
typedef struct NoArvore {
    char *palavra;
    int ocorrencias;
    NoLinha *listaLinhas;
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;

// estrutura para armazenar o texto original
typedef struct {
    char **linhas;  // array de strings
    int totalLinhas;
    int capacidade; // para redimensionamento, se preciso
} TextoMemoria;


// funcoes auxiliares
char* processar_palavra(char* crua);
void adicionar_linha(NoLinha **raiz, int linha);
void carregar_arquivo(char *nomeArquivo, TextoMemoria *texto, void **indice, char *tipoIndice, int *comp);

// funcoes para lista
void inserir_lista(NoLista **inicio, char *palavra, int linha, int *comp);
NoLista* buscar_lista(NoLista *inicio, char *palavra, int *comp);
void liberar_lista(NoLista *inicio);

// funcoes para arvore
void inserir_arvore(NoArvore **raiz, char *palavra, int linha, int *comp);
NoArvore* buscar_arvore(NoArvore *raiz, char *palavra, int *comp);
int altura_arvore(NoArvore *raiz);
void liberar_arvore(NoArvore *raiz);
int contar_palavras_unicas_arvore(NoArvore *raiz); // auxiliar para estatisticas
int contar_palavras_unicas_lista(NoLista *inicio);

int main(int argc, char *argv[]) {

    // validacao dos argumentos
    if (argc != 3) {
        printf("Quantidade de argumentos incorreta!");
        return 1;
    }

    char *nomeArquivo = argv[1];
    char *tipoIndice = argv[2];
    
    // validar se o tipo eh "lista" ou "arvore"
    if (strcmp(tipoIndice, "lista") != 0 && strcmp(tipoIndice, "arvore") != 0) {
        printf("O tipo de indice deve ser 'lista' ou 'arvore'.\n");
        return 1;
    }

    // inicializacao das estruturas
    TextoMemoria texto;
    texto.linhas = NULL; // inicialize adequadamente (malloc)
    texto.totalLinhas = 0;
    texto.capacidade = 0;
    
    NoLista *indiceLista = NULL;
    NoArvore *indiceArvore = NULL;

    int comparacoesConstrucao = 0;

    // carregamento do arquivo e construcao do indice
    // a logica de leitura deve estar aqui ou numa função separada

    // simulacao da chamada da funcao:
    carregar_arquivo(nomeArquivo, &texto, (strcmp(tipoIndice, "lista") == 0) ? (void*)&indiceLista : (void*)&indiceArvore, tipoIndice, (void*)&comparacoesConstrucao);
    
    // exibicao das estatisticas iniciais
    printf("Arquivo: '%s'\n", nomeArquivo);
    printf("Tipo de indice: '%s'\n", tipoIndice);
    printf("Numero de linhas no arquivo: %d\n", texto.totalLinhas); // atualizar com valor real
    
    int totalPalavras = 0;
    if (strcmp(tipoIndice, "lista") == 0) 
        totalPalavras = contar_palavras_unicas_lista(indiceLista);
    else 
        totalPalavras = contar_palavras_unicas_arvore(indiceArvore);

    printf("Total de palavras unicas indexadas: %d\n", totalPalavras);

    if (strcmp(tipoIndice, "arvore") == 0) {
        printf("Altura da arvore: %d\n", altura_arvore(indiceArvore));
    }
    
    printf("Numero de comparacoes realizadas para a construcao do indice: %d\n", comparacoesConstrucao);


    // loop de busca
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
            
            // TODO: normalizar palavraBusca (minúsculas) antes de buscar
            int compBusca = 0;
            NoLinha *resultadoLinhas = NULL;
            int totalOcorrencias = 0;

            // executar busca
            if (strcmp(tipoIndice, "lista") == 0) {
                NoLista *res = buscar_lista(indiceLista, palavraBusca, &compBusca);
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

            // exibir resultados
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
            // consumir o resto da linha se comando for inválido
            char lixo[100]; fgets(lixo, 100, stdin);
            printf("Opcao invalida!\n");
        }
    }

    // TODO: Liberar memória alocada (texto, indice e listas de linhas)

    return 0;
}

// implementacao das funcoes (para codificarmos)

void carregar_arquivo(char *nomeArquivo, TextoMemoria *texto, void **indice, char *tipoIndice, int *comp) {
    FILE *pont_arq;
    pont_arq = fopen(nomeArquivo, "r");

    if (pont_arq == NULL) {
        printf("Erro ao abrir o arquivo!");
        return 1;
    }

    char buffer[2000]; // buffer temporario para ler cada linha

    // para aumentar o tamanho se o texto estiver cheio
    while (fgets(buffer, sizeof(buffer), pont_arq) != NULL) {
        if (texto->totalLinhas == texto->capacidade) {
            if (texto->capacidade == 0) texto->capacidade = 10;
            texto->capacidade *= 2;
        }

        texto->linhas[texto->totalLinhas] = strdup(buffer);
        int linhaAtual = texto->totalLinhas + 1;
        texto->totalLinhas++;

        
    char palavraTemp[200]; // Buffer para montar uma palavra
        int p = 0; // Índice do buffer da palavra

        // Percorre cada caractere da linha que acabamos de ler
        for (int i = 0; buffer[i] != '\0'; i++) {
            char c = buffer[i];

            // Se for letra ou número, adiciona ao buffer da palavra
            if (isalnum(c)) {
                palavraTemp[p++] = tolower(c); // Converte para minúscula aqui mesmo [cite: 63]
            } 
            // Se for separador (espaço, pontuação, hífen[cite: 61], quebra de linha)
            // E se já tivermos acumulado alguma palavra no buffer
            else if (p > 0) {
                palavraTemp[p] = '\0'; // Finaliza a string da palavra

                // --- ETAPA 3: Inserir no Índice ---
                if (strcmp(tipoIndice, "lista") == 0) {
                    // Cast explicito: (NoLista**) diz que o void** é na verdade um ponteiro de lista
                    inserirLista((NoLista**)indice, palavraTemp, linhaAtual, comp);
                } else {
                    inserirArvore((NoArvore**)indice, palavraTemp, linhaAtual, comp);
                }

                p = 0; // Reseta o buffer para a próxima palavra
            }
        }
        
        // Caso a linha termine com uma palavra sem pontuação depois (raro com fgets, mas possível)
        if (p > 0) {
            palavraTemp[p] = '\0';
            if (strcmp(tipoIndice, "lista") == 0) {
                inserirLista((NoLista**)indice, palavraTemp, linhaAtual, comp);
            } else {
                inserirArvore((NoArvore**)indice, palavraTemp, linhaAtual, comp);
            }
        }
    }

    fclose(pont_arq);
}

void inserir_lista(NoLista **inicio, char *palavra, int linha, int *comp) {
    // TODO: implementar inserção na lista
    // lembre-se de incrementar *comp a cada strcmp realizado
}

void inserir_arvore(NoArvore **raiz, char *palavra, int linha, int *comp) {
    // TODO: implementar inserção na árvore BST
    // use strcasecmp ou strcmp (se já estiver minúsculo) e incremente *comp
}

// adicionar as demais funcoes aqui