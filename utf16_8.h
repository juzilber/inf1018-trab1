#include <stdio.h>

/*
    Interface utf16_8 - Implementador: Gabriel Barbosa

    Descricao: Permite acessar a funcao utf16_8, que le de um arquivo
               em UTF-16 e reescreve em UTF-8.

    Parametro: arq_entrada - Endereco do arquivo de leitura.
               arq_saida - Endereco do arquivo de escrita.

    Retorno: 0 - Caso a execucao seja correta.
            -1 - Caso a haja erros na execucao.
*/

int utf16_8(FILE* arq_entrada, FILE* arq_saida);
