#include <stdio.h>

int getFirst (int i){
    return (i & 0xFFFF0000) >> 16;
}

int getSecond (int i){
    return (i & 0x0000FFFF);
}

int TypeChar (int i){
    int first = getFirst(i);
    int second = getSecond(i);
    if(first >= 0xD800 && first<= 0xDFFF && second >= 0xD800 && second <= 0xDFFF){
        return 1; //Sao 2 code units de 1 elemento
    } else {
        return 2; //Sao 2 code units de 2 elementos
    }
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

int decompCodeUnits(int i){
    int first = i & 0x03FF0000;
    int second = i & 0x000003FF;
    return ((first >> 6) | second) + 0x10000;
}

int main(void){
    int i = 0xD801DFFE; //2 Code Units - 2 Elementos
    int p = 0xD800FFFF; //2 Code Units - 1 Elemento
    //Separando o primeiro inteiro em 2 bytes
    int primeiro = getFirst(i);
    int segundo = getSecond(i);
    //Determinando os brackets de cada metade do inteiro
    int bracket1 = bracketUtf8(primeiro);
    int bracket2 = bracketUtf8(segundo);
    //Determina quantos elementos os 2 code units representam
    int type = TypeChar(i);
    int type_p = TypeChar(p);
    //Como i representa 1 elemento, decompomos os 20 bits
    //Esse tem que ser simulado na mao para ver sua corretude
    int decomp_i = decompCodeUnits(i);
    //Printing...
    printf("Bytes:\n");
    printf("%x - 1o\n%x - 2o\n", primeiro, segundo);
    printf("Brackets dos Bytes:\n");
    printf("%d - 1o\n%d - 2o\n", bracket1, bracket2);
    printf("Tipos dos inteiros:\n");
    printf("%d - Tipo I\n%d - Tipo P\n", type, type_p);
    printf("Decomposicao I:\n");
    printf("%x\n", decomp_i);
    return 0;
}
