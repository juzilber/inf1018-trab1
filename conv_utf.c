/*
Gabriel Diniz Junqueira Barbosa - 1511211 - 3WB
Juliana Zilberberg - 1410899 - 3WB
*/

#include <stdio.h>
#include <stdlib.h>

unsigned int transform_utf16(unsigned int unicode) {
	if (unicode >= 0 && unicode <= 0xffff)
		return unicode;
	else {
		unsigned int utf16 = unicode - 0x10000;
		printf("utf = %4x", utf16);
		unsigned int utf16_1 = (utf16 >> 10) + 0xd800;
		unsigned int utf16_2 = (utf16 & 0x3ff) + 0xdc00;
		return (utf16_1 << 16) | utf16_2;
	}
}

void printBigEndian(unsigned int utf, FILE *arq_saida) {
	if ((utf & 0xffff0000) == 0) { //apenas um code unit
		unsigned short new_utf = (unsigned short)utf;
		unsigned char x = ((unsigned char)(new_utf >> 8));
		fwrite(&x, 1, 1, arq_saida);
		x = ((unsigned char)((new_utf << 8) >> 8));
		fwrite(&x, 1, 1, arq_saida);
	}
	else { //dois code unit
		unsigned char x = ((unsigned char)(utf >> 24));
		fwrite(&x, 1, 1, arq_saida);
		x = ((unsigned char)((utf << 8) >> 24));
		fwrite(&x, 1, 1, arq_saida);
		x = ((unsigned char)((utf << 16) >> 24));
		fwrite(&x, 1, 1, arq_saida);
		x = ((unsigned char)((utf << 24) >> 24));
		fwrite(&x, 1, 1, arq_saida);
	}
}

void print_unicode(FILE *arq_entrada, FILE *arq_saida) {
	unsigned char c = fgetc(arq_entrada);
	while (!feof(arq_entrada)) {
		if ((c & 0x80) == 0) { //compara para ver se o primeiro digito eh 0
			//printf("entrou no primeiro caso: %.4x\n", c);
			printf("sh = %x\n", transform_utf16((unsigned int)c));
			//fputc(transform_utf16((unsigned int)c), arq_saida);
			printBigEndian(transform_utf16((unsigned int)c), arq_saida);
		}
		else if ((c & 0x20) == 0) { // compara para ver se o terceiro digito eh 0
			//printf("entrou no segundo caso, pega mais um char: %x\n", c);
			c = c & 0x1f;
			unsigned int sh = (unsigned int)c;
			sh = sh << 6;
			unsigned char continuacao = fgetc(arq_entrada);
			continuacao = continuacao & 0x3f;
			sh = sh | continuacao;
			printf("sh = %4x\n", sh);
			printf("sh = %x\n", transform_utf16(sh));
			//fputc(transform_utf16(sh), arq_saida);
			printBigEndian(transform_utf16(sh), arq_saida);
		}
		else if ((c & 0x10) == 0) { //compara para ver se o quarto digito eh 0
			//printf("entrou no terceiro caso, pega mais dois chars: %x\n", c);
			c = c & 0x0f;
			unsigned int sh = (unsigned int)c;
			sh = sh << 12;
			unsigned int continuacao = fgetc(arq_entrada);
			continuacao = continuacao & 0x3f;
			continuacao = continuacao << 6;
			sh = sh | continuacao;
			continuacao = fgetc(arq_entrada);
			continuacao = continuacao & 0x3f;
			sh = sh | continuacao;
			printf("sh = %4x\n", sh);
			printf("sh = %x\n", transform_utf16(sh));
			//fputc(transform_utf16(sh), arq_saida);
			printBigEndian(transform_utf16(sh), arq_saida);
		}
		else {
			//printf("entrou no quarto caso, pega mais tres chars: %x\n", c);
			c = c & 0x07;
			unsigned int sh = (unsigned int)c;
			sh = sh << 18;
			unsigned int continuacao = fgetc(arq_entrada);
			continuacao = (continuacao & 0x3f) << 12;
			sh = sh | continuacao;
			continuacao = fgetc(arq_entrada);
			continuacao = (continuacao & 0x3f) << 6;
			sh = sh | continuacao;
			continuacao = fgetc(arq_entrada);
			continuacao = continuacao & 0x3f;
			sh = sh | continuacao;
			printf("sh = %4x\n", sh);
			printf("sh = %x\n", transform_utf16(sh));
			printBigEndian(transform_utf16(sh), arq_saida);
		}
		c = fgetc(arq_entrada);
	}
}

int utf8_16(FILE *arq_entrada, FILE *arq_saida) {
	if (arq_entrada == NULL || arq_saida == NULL) {
		//todo: emitir erro na saida (stderr)
		return -1;
	}
	fputc(0xfe, arq_saida);
	fputc(0xff, arq_saida);
	print_unicode(arq_entrada, arq_saida);
	
	return 0;
}

int main(void) {
	FILE *e = fopen("entrada.txt", "rb");
	FILE *s = fopen("saida.txt", "wb");
	utf8_16(e, s);
	fclose(s);
	s = NULL;
	s = fopen("saida.txt", "rb");
	unsigned char c = fgetc(s);
	while (!feof(s))
	{
		printf("\n%x", c);
		c = fgetc(s);
	}
	fclose(e);
	fclose(s);
	return 0;
}
