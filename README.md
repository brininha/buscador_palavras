<div align="center">
<img width="200" alt="ilustracao" src="https://github.com/user-attachments/assets/c970ea4c-d533-48e7-8176-96aebebb6e99" />
</div>

<h1 align="center"> Indexador e buscador de palavras </h1>

Disciplina: Algoritmos e Estruturas de Dados I  

## Sobre o projeto
Este projeto implementa um sistema de indexação e busca de palavras em arquivos de texto, desenvolvido em linguagem C. O objetivo principal é analisar e comparar o desempenho entre duas estruturas de dados distintas:

- Lista sequencial
- Árvore binária de busca

O sistema calcula o número de comparações realizadas tanto na construção do índice quanto na busca de termos, permitindo validar as complexidades teóricas $O(n)$ e $O(\log n)$.

## Funcionalidades
* **Indexação de textos:** leitura de arquivos `.txt` e criação de índices em memória.
* **Busca interativa:** interface via terminal para buscar palavras e visualizar as linhas onde ocorrem.
* **Métricas de desempenho:** relatório imediato do número de comparações realizadas a cada operação.

## Como compilar

Para compilar o projeto, certifique-se de ter o compilador `gcc` instalado. Execute o comando abaixo no terminal:

```
gcc main.c -o ep
``` 

## Como executar

O programa deve ser executado via linha de comando, recebendo dois argumentos obrigatórios:

```
./ep <caminho_do_arquivo> <tipo_de_indice>
```

- `<caminho_do_arquivo>`: o arquivo de texto a ser lido.
- `<tipo_de_indice>`: escolha entre lista ou arvore.

**Exemplo:**
```
./ep amostras/texto1.txt lista
```

## Interações

Após carregar o índice, o programa entrará em loop de busca. Digite os comandos:

- `busca <palavra>`: procura a palavra no índice.

- `fim`: encerra o programa.

**Exemplo:**

```
> busca algorithm
Existem 3 ocorrências da palavra 'algorithm' na(s) seguinte(s) linha(s):
00001: Informally, an algorithm is any well-defined...
00003: We can also view an algorithm as a tool...
Numero de comparacoes: 12

> fim
```

## Autoria
Esse programa foi desenvolvido por [Manuela Campos de Amorim](https://github.com/manuolivercam) e [Sabrina Cristan](https://github.com/brininha).
