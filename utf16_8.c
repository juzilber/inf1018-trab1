/*
Gabriel Diniz Junqueira Barbosa - 1511211 - 3WB
Juliana Zibenberg - Matricula - 3WB
*/
#include "utf16_8.h"


/*
    Funcao Privada BOM_BE - Implementador: Gabriel Barbosa

    Descricao: Essa funcao determina se o BOM indica uma ordenacao
               Big Endian. Se for, retorna 0, caso contrario, 1
               e retornado.

    Paramentros: BOM - Valor do BOM do arquivo

    Retornos: 0 - Caso o BOM indicar uma ordenacao big endian.
              1 - Caso o BOM indicar uma ordenacao diferente.
*/

int BOM_BE (short BOM){
    int i = 0xFEFF;
    if ((BOM - i) != 0)
        return 0;
    return 1;
}

/*
    Funcao Privada TypeChar - Implementador: Gabriel Barbosa

    Descricao: Para cada caracter codificado, se identifica quantos
               code units esse caracter possui.

    Parametro: Recebe um unsigned int que representa o primeiro
               byte do caracter.

    Retorno: 1 - Caso o inteiro seja composto pelos dois code units
                 de um caracter.
             2 - Caso o caracter seja composto por dois code units
                 representando dois caracteres diferentes.
*/

int TypeChar (int i){
    int first = getFirst(i);
    int second = getSecond(i);
    if(first >= 0xD800 && first<= 0xDFFF && second >= 0xD800 && second <= 0xDFFF){
        return 1; //Sao 2 code units de 1 elemento
    } else {
        return 2; //Sao 2 code units de 2 elementos
    }
}

/*
    Funcoes Privadas getFirst e getSecond
    
    Implementador: Gabriel Barbosa

    Descricao: Recebendo o inteiro, divide ele em dois inteiros,
               para que seja transformado em dois caracteres.

    Parametro: Recebe um int a ser dividido.

    Retorno: Os numeros que representam os dois inteiros contidos
             no inteiro.
*/

int getFirst (int i){
    return (i & 0xFF00) >> 16;
}

int getSecond (int i){
    return (i & 0x00FF);
}

/*
    Funcao Privada bracketUtf8 - Implementador: Gabriel Barbosa

    Descricao: Identifica em qual bracket da representacao UTF-8
               esse inteiro esta localizado.

    Parametro: Recebe um unsigned int que representa o valor que
               sera representado em UTF-8.

    Retorno: 1 - Caso se encontre entre 0x0000 e 0x007F
             2 - Caso se encontre entre 0x0080 e 0X07FF
             3 - Caso se encontre entre 0x0800 e 0xFFFF
             4 - Caso se encontre entre 0x10000 e 0x10FFFF
*/

int bracketUtf8(int i){
    if(i <= 0x007F){
        return 1;
    } else if(i <= 0x07FF){
        return 2;
    } else if (i <= 0xFFFF){
        return 3;
    } else if (i <= 0x10FFFF){
        return 4;
    } else {
        return -1;
    }
}

/*
    Funcao Privada writeUFT8 - Implementador: Gabriel Barbosa

    Descricao: Recebe o valor UNICODE do caracter a ser escrito em
               UTF-8.

    Parametro: Recebe o inteiro a ser escrito e o endereco do file
               em que serao escritos os caracteres.

    Retorno: 0 - Caso a execucao seja correta.
            -1 - Caso a haja erros na execucao.
*/

int writeUTF8 (int val, FILE* arq_saida){
    int bracket = bracketUtf8(val);
    if(bracket == 1){
        char c = val & 0x7F;
        if(fwrite(&c, sizeof(char), 1, arq_saida) == 0){
            printf("Erro de escrita!");
            return -1;
        }
    } else if(bracket == 2){
        short first = 0xC000 | (val & 0x1F00);
        short second = 0x0080 | (val & 0x003F);
        short s = first | second;
        if(fwrite(&s, sizeof(short), 1, arq_saida) == 0){
            printf("Erro de escrita!");
            return -1;
        }
    } else if(bracket == 3){
        int first = 0xE00000 | (val & 0x0F0000);
        int second = 0x008000 | (val & 0x003F00);
        int third = 0x000080 | (val & 0x00003F);
        int i = first| second | third;
        if(fwrite(&i, sizeof(int), 1, arq_saida) == 0){
            printf("Erro de escrita!");
            return -1;
        }
    } else if(bracket == 4){
        int first = 0xF0000000 | (val & 0x07000000);
        int second = 0x00800000 | (val & 0x003F0000);
        int third = 0x00008000 | (val & 0x00003F00);
        int fourth = 0x00000080 | (val & 0x0000003F);
        int i = first | second | third | fourth;
        if(fwrite(&i, sizeof(int), 1, arq_saida) == 0){
            printf("Erro de escrita!");
            return -1;
        }
    }
    return 0;
}

/*
    Funcao Privada decompCodeUnits
    
    Implementador: Gabriel Barbosa

    Descricao: Recebe o inteiro que contem os dois code units que
               contem os 20 bits mais significantes.

    Parametro: Recebe o inteiro a ser restaurado aos 20 bits.

    Retorno: Os 20 bits significantes antes das operacoes.
*/

int decompCodeUnits(int i){
    int first = i & 0x03FF0000;
    int second = i & 0x000003FF;
    return ((first >> 6) | second) + 0x10000;
}

/*
    Funcao Publica utf16_8 - Implementador: Gabriel Barbosa

    Descricao: Recebe o endereco de um arquivo codificado em
               UTF-16 que sera reescrito no outro arquivo cujo
               endereco e passado.

    Parametro: arq_entrada - Endereco do arquivo de leitura.
               arq_saida - Endereco do arquivo de escrita.

    Retorno: 0 - Caso a execucao seja correta.
            -1 - Caso a haja erros na execucao.
*/

int utf16_8(FILE* arq_entrada, FILE* arq_saida){
    int i = 0; num = 0;
    num = fread(&i, sizeof(int), 1, arq_entrada);
    while(num != EOF){
        if(num == 0){
            printf("Erro de leitura!");
            return -1;
        } 
        if(TypeChar(i) == 2){
            int first = getFirst(i);
            int second = getSecond(i);
            if(writeUTF8 (first, arq_saida) == -1)
                return -1;
            if(writeUTF8 (second, arq_saida) == -1)
                return -1; 
        } else if(TypeChar(i) == 1){
            int val = decompCodeUnits(i);
            if(writeUTF8 (val, arq_saida) == -1)
                return -1;
        }
    }
    return 0;
}


