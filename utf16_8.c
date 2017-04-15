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

int getFirst (int i){
    return (i & 0xFF00) >> 16;
}

int getSecond (int i){
    return (i & 0x00FF);
}

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

int utf16_8(FILE* arq_entrada, FILE* arq_saida){
    int i = 0; num = 0;
    num = fread(&i, sizeof(int), 1, arq_entrada);
    if(num == 0){
        printf("Erro de leitura!");
        return -1;
    } 
    while(num != EOF){
        if(TypeChar(i) == 2){
            int first = getFirst(i);
            int second = getSecond(i);
            if (bracket)
        }
    }
    return 0;
}


