#include <stdio.h>
#include "conv_utf.h"

//Arquivo de teste da funcao utf16_8

int main(void){

    //Teste de conversao UTF 16 para UTF 8
    /*FILE * arq_entrada = fopen("arqEntrada.txt", "rb");
    FILE * arq_saida = fopen("arqSaida.txt", "wb");

    int ret = utf16_8(arq_entrada, arq_saida);
    
    if(ret == -1)
        printf("Houve erro!\n");
    else
        printf("Execucao correta!\n");

    fclose(arq_entrada);
    fclose(arq_saida);*/

    //Teste de conversao UTF 8 para UTF 16
    FILE *arq_entrada  = fopen("entrada.txt", "rb");
    FILE *arq_saida = fopen("saida.txt", "wb");
    utf8_16(arq_entrada, arq_saida);
    fclose(arq_entrada);
    fclose(arq_saida);

    return 0;
}
