/*
Gabriel Diniz Junqueira Barbosa - 1511211 - 3WB
Juliana Zibenberg - Matricula - 3WB
*/

#include "utf16_8.h"
#include <stdio.h>

/*
    Funcao Privada BOM_BE - Implementador: Gabriel Barbosa

    Descricao: Essa funcao determina se o BOM indica uma ordenacao
               Big Endian. Se for, retorna 1, caso contrario, 0
               e retornado.

    Paramentros: BOM - Valor do BOM do arquivo

    Retornos: 1 - Caso o BOM indicar uma ordenacao big endian.
              0 - Caso o BOM indicar uma ordenacao diferente.
*/

int BOM_BE (unsigned short BOM){
    unsigned short i = 0xFEFF;
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

int TypeChar (unsigned short s){
    unsigned short s1 = 0xD800, s2 = 0xDFFF;
    if(s >= s1 && s<= s2){
        return 1; //Sao 2 code units de 1 elemento
    } else {
        return 2; //Sao 2 code units de 2 elementos
    }
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

int bracketUtf8(unsigned int i){
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

int writeUTF8 (unsigned int val, FILE* arq_saida){
    int bracket = bracketUtf8(val);
    if(bracket == 1){
        unsigned char c = val & 0x7F;
        if(fwrite(&c, sizeof(unsigned char), 1, arq_saida) == 0){
            printf("Erro de escrita!");
            return -1;
        }
    } else if(bracket == 2){
        unsigned short first = 0xC000 | ((val & 0x07C0) << 2);
        unsigned short second = 0x0080 | (val & 0x003F);
        unsigned short s = (first >> 8) | (second << 8);

        if(fwrite(&s, sizeof(unsigned short), 1, arq_saida) == 0){
            printf("Erro de escrita!");
            return -1;
        }
    } else if(bracket == 3){
        unsigned char c[3];
        unsigned int first = 0xE00000 | ((val & 0x00F000) << 4);
        unsigned int second = 0x008000 | ((val & 0x000FC0) << 2);
        unsigned int third = 0x000080 | (val & 0x00003F);

        c[0] = first >> 16;
        c[1] = second >> 8;
        c[2] = third;
        

        fwrite(&c[0], sizeof(unsigned char), 1, arq_saida);
        fwrite(&c[1], sizeof(unsigned char), 1, arq_saida);
        fwrite(&c[2], sizeof(unsigned char), 1, arq_saida);
        
    } else if(bracket == 4){

        unsigned int first = 0xF0000000 | ((val & 0x001C00000) << 6);
        unsigned int second = 0x00800000 | ((val & 0x0003F000) << 4);
        unsigned int third = 0x00008000 | ((val & 0x00000FC0) << 2);
        unsigned int fourth = 0x00000080 | (val & 0x0000003F);

        unsigned int i = first >> 24 | second >> 8 | third << 8 | fourth << 24; //Inverte a ordem pois eh big endian!!

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

unsigned int decompCodeUnits(unsigned short a, unsigned short b){
    unsigned short s1 = a & 0x03FF;
    unsigned short s2 = b & 0x03FF;
    unsigned int i1 = s1;
    i1 = i1 << 10;
    unsigned int i2 = s2;
    unsigned int i = (i1 | i2) + 0x10000;
    return i;
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

    unsigned short first = 0, second = 0, bom = 0;
    int num = 0;

    num = fread(&bom, sizeof(unsigned short), 1, arq_entrada);

    if(num != 1){
        printf("Erro de leitura do BOM!\n");
        return -1;
    }

    if (BOM_BE(bom) == 0){ //Checa o BOM
        printf("BOM nao indica big endian!\n");
        return -1;
    }

    num = fread(&first, sizeof(unsigned short), 1, arq_entrada);

    while(!feof(arq_entrada)){

        if(num != 1){ //Checa a leitura do primeiro short
            printf("Erro de leitura!\n");
            return -1;
        }

        if(TypeChar(first) == 2){ // 1 short - 1 elemento
            unsigned int val = 0 | first;
            if(writeUTF8 (val, arq_saida) == -1){
                printf("Erro de escrita!!\n");
                return -1; //Erro de Escrita
            }

        } else if(TypeChar(first) == 1){ // 2 short - 1 elemento
            
            // Leitura de mais um short
            num = fread(&second, sizeof(short), 1, arq_entrada);

            if(num != 1){ //Checa a leitura do segundo short
                printf("Erro de leitura!\n");
                return -1;
            }

            unsigned int val = decompCodeUnits(first, second);

            if(writeUTF8 (val, arq_saida) == -1){
                return -1; //Erro de Escrita
                printf("Erro de escrita!\n");
            }
        }

        num = fread(&first, sizeof(short), 1, arq_entrada);
    }

    return 0;

}


