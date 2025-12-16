Integrantes: 
Manuela Campos de Amorim | NUSP: 16887191
Sabrina Cristan | NUSP: 16837632

Instruções de como rodar o EP (você também pode acessar o arquivo README.md que está mais formatado):

1. Compilação

Para compilar o programa, utilize um compilador GCC padrão. No terminal, 
execute o comando:

    gcc main.c -o ep

Isso irá gerar o executável 'ep'.

2. Execução

O programa deve ser executado via linha de comando recebendo dois 
argumentos obrigatórios:

    ./ep <caminho_do_arquivo> <tipo_de_indice>

Onde <tipo_de_indice> deve ser "lista" ou "arvore".

Exemplos de execução:

    ./ep amostras/texto1.txt lista
    ./ep amostras/texto4.txt arvore

3. Interações

Ao iniciar, o programa carregará o índice e aguardará comandos do usuário:

    > busca <palavra>  : Busca uma palavra e exibe suas ocorrências.
    > fim              : Encerra a execução do programa.

Exemplo:

    > busca algorithm
    > busca the
    > fim
