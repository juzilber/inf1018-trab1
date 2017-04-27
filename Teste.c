#include <stdio.h>
#include "utf16_8.h"

//Arquivo de teste da funcao utf16_8

int main(void){
    FILE * arq_entrada = fopen("arqEntrada.txt", "rb");
    FILE * arq_saida = fopen("arqSaida.txt", "wb");

    int ret = utf16_8(arq_entrada, arq_saida);
    
    if(ret == -1)
        printf("Houve erro!\n");
    else
        printf("Execucao correta!\n");

    fclose(arq_entrada);
    fclose(arq_saida);

    return 0;
}
