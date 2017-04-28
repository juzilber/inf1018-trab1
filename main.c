#include <stdio.h>
#include "conv_utf.h"

//Arquivo de teste da funcao utf16_8

int main(void){

    //Teste de conversao UTF 16 para UTF 8
    FILE * arq_entrada1 = fopen("arqEntrada.txt", "rb");
    FILE * arq_saida1 = fopen("arqSaida.txt", "wb");

    int ret1 = utf16_8(arq_entrada1, arq_saida1);
    int ret2;
    
    fclose(arq_entrada1);
    fclose(arq_saida1);

    //Teste de conversao UTF 8 para UTF 16
    FILE *arq_entrada2  = fopen("entrada.txt", "rb");
    FILE *arq_saida2 = fopen("saida.txt", "wb");
    ret2 = utf8_16(arq_entrada2, arq_saida2);
    fclose(arq_entrada2);
    fclose(arq_saida2);

    if(ret1 == -1 && ret2 == -1)
        printf("Houve erro!\n");
    else
        printf("Execucao correta!\n");

    return 0;
}
