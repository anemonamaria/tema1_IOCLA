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
	char *ptr;

	ptr = &buff[49];
	*ptr = '\0';

	do
	{
		*--ptr = reprezentanti[numar % baza];
		numar = numar / baza;
	} while (numar != 0);

	return ptr;
}

int iocla_printf(const char *format, ...)
{
	const char *tok;
	long long int i;
	char *s;
	int len = 0;

	va_list arg;
	va_start(arg, format);
	for(tok = format; *tok != '\0'; tok++)
	{
		while( *tok != '%')
		{
			write_stdout(tok, 1);
			len++;
			tok++;
			if(*tok == '\0') goto exit;
		}
		char b ='%';

		tok++;
		if (*tok == '%') {
			write_stdout(&b,1);
			len = len + 1;
		}

		switch(*tok)
		{
			case 'c' : i = va_arg(arg,int);
						char c = i;
						write_stdout(&c, 1);
						len	= len + 1;

						break;

			case 'd' : i = va_arg(arg,int);

						if(i < 0)
						{
							i = -i;
							char a = '-';
							write_stdout(&a, 1);
							len++;
						}
						write_stdout(reprezentare(i,10), strlen(reprezentare(i,10)));
						len	= len + strlen(reprezentare(i,10));
						break;

			case 'u' : i = va_arg(arg, unsigned int);
						char *p;
						if(i<0)
							p = (reprezentare(UINT_MAX-i+1,10));
						else p =(reprezentare(i, 10));

						write_stdout(p, strlen(p));
						len = len +strlen(p);
						break;

			case 's': s = va_arg(arg,char *);
						write_stdout(s,strlen(s));
						len = len + strlen(s);

						break;

			case 'x': i = va_arg(arg,unsigned int);
						write_stdout(reprezentare(i,16), strlen(reprezentare(i,16)));
						len = len + strlen(reprezentare(i,16));

						break;
		}
	}
	exit:

	va_end(arg);
	if (len!=0) return len;
	else return -1;
}
