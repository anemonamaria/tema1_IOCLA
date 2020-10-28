#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

char *reprezentare(long long int numar, int baza)
{
	// functie in care transformam un numar dintr-o baza in alta
	char reprezentanti[] = "0123456789abcdef";
	static char buff[100];
	char *pointer;

	pointer = &buff[50];
	*pointer = '\0';

	do {
		// in functie de rest se va retine in pointer numarul convertit
		// in baza ceruta
		int rest = numar % baza;
		*--pointer = reprezentanti[rest];
		numar = numar / baza;
	} while (numar != 0);

	return pointer;  // returnam numarul convertit
}

int iocla_printf(const char *format, ...)
{
	const char *tok;
	char *str;
	long long int i;
	int len = 0;

	va_list arg;
	va_start(arg, format);

	for(tok = format; *tok != '\0'; tok++) {
		while( *tok != '%') {
			// printam toate caracterele intalnite pana la %
			write_stdout(tok, 1);
			len++;  // numaram cate caractere printam
			tok++;  // trecem la urmatorul caracter
			if(*tok == '\0') goto exit;
		}
		char b ='%';

		tok++;  // trecem la urmatorul caracter
		if (*tok == '%') {
			// in cazul in care caracterul urmator este tot % il com afisa
			write_stdout(&b,1);
			len = len + 1;  // si il vom adauga la numaratoare
		}

		switch(*tok) {
			// in functie de argumentul operatorului % vom afisa ce se
			// cere si vom contoriza numarul caracterelor afisate
			case 'd' : i = va_arg(arg,int);
					// %d întreg, convertit la reprezentare zecimală, cu semn
					if(i < 0) {
						i = -i;
						char a = '-';

						write_stdout(&a, 1);
						len++;
					}
					int lungime1 = strlen(reprezentare(i,10));

					write_stdout(reprezentare(i,10), lungime1);
					len	= len + strlen(reprezentare(i,10));
					break;

			case 'u' : i = va_arg(arg, unsigned int);
					// %u întreg, convertit la reprezentare zecimală, fara semn
					char *p;

					if(i<0)
						p = (reprezentare(UINT_MAX-i+1,10));
					else p =(reprezentare(i, 10));
						write_stdout(p, strlen(p));
					len = len +strlen(p);
					break;

			case 'x': i = va_arg(arg,unsigned int);
					// %x întreg, convertit la reprezentare hexazecimală, fara
					// semn
					int lungime2 = strlen(reprezentare(i,16));

					write_stdout(reprezentare(i,16), lungime2);
					len = len + strlen(reprezentare(i,16));
					break;

			case 'c' : i = va_arg(arg,int);
					// %c caracter, convertit la reprezentarea ASCII
					char c = i;

					write_stdout(&c, 1);
					len	= len + 1;
					break;

			case 's': str = va_arg(arg,char *);
					// %s pointer la un șir de caractere, ce va fi afișat
					// neschimbat
					write_stdout(str,strlen(str));
					len = len + strlen(str);
					break;
		}
	}
	exit:
	va_end(arg);

	// daca s-a printat ceva vom returna lungimea sirului
	if (len!=0) return len;
	else return -1;
}
