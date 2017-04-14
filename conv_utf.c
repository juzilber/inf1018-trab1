#include <stdio.h>

void print_unicode(FILE *arq_entrada) {
	unsigned char c = fgetc(arq_entrada);
	while (!feof(arq_entrada)) {
		if ((c & 0x80) == 0) { //compara para ver se o primeiro digito eh 0
			printf("entrou no primeiro caso: %.4x\n", c);
		}
		else if ((c & 0x20) == 0) { // compara para ver se o terceiro digito eh 0
			printf("entrou no segundo caso, pega mais um char: %x\n", c);
			c = c & 0x1f;
			unsigned  sh = (unsigned int)c;
			sh = sh << 6;
			unsigned char continuacao = fgetc(arq_entrada);
			continuacao = continuacao & 0x3f;
			sh = sh | continuacao;
			printf("sh = %.4x\n", sh);
		}
		else if ((c & 0x10) == 0) { //compara para ver se o quarto digito eh 0
			printf("entrou no terceiro caso, pega mais dois chars: %x\n", c);
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
			printf("sh = %x\n", sh);
		}
		else {
			printf("entrou no quarto caso, pega mais tres chars: %x\n", c);
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
			printf("sh = %x\n", sh);
		}
		c = fgetc(arq_entrada);
	}
}

int utf8_16(FILE *arq_entrada, FILE *arq_saida) {
	if (arq_entrada == NULL || arq_saida == NULL) {
		//todo: emitir erro na saida (stderr)
		return -1;
	}
	print_unicode(arq_entrada);
	return 0;
}

int main(void) {
	FILE *e = fopen("entrada.txt", "rb");
	FILE *s = fopen("saida.txt", "wb");
	utf8_16(e, s);
	return 0;
}