/* BONIX - parser.c
 * 2008 - Felipe Astroza
 * Under GPLv3 (see LICENSE)
 */

#include <stdio.h>
#define __USE_ISOC99
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMPONENTS 15

typedef struct
{
        int count;
        char *component[MAX_COMPONENTS];
        char *src_nick;
        char *src_user;
        char *src_host;
} bonix_message;

#define BUFSIZE 4096
#define RERROR 	1

void parsing(char *buf, int len, bonix_message *ret)
{
	char *token = buf, *tmp = buf, *end;
	int count = 0;

	end = buf + len - 2;
	/* Segun rfc 1459 los mensajes deben terminar con un par \r\n, en este caso
	* al que encuentre primero (\r) lo cambiare por un espacio, de esta forma (1) no tendra
	* problemas con encontrar el ultimo parametro.
	*/

	if(*end == '\r')
		*end = ' ';
	else
		end = NULL;
		
	memset(ret, 0, sizeof(bonix_message));

	/* Detectando origen (prefijo) */
	if(*buf == ':') {
		while((*token++)) {
			if(*token == ' ') { /* Corto y lo agrego a la lista de componentes de mensaje. */
				*token = 0;
				ret->component[count++] = tmp + 1;
				tmp = (token = token + 1);
				break;
			}
		}
	}

	/* (1) */
	while(*token) {
		if(*token == ' ' && *(token + 1) != ' ') {
			*token = 0; /* Corta la cadena */
			ret->component[count++] = tmp;
			if(*(token + 1) == ':') {
				ret->component[count++] = token + 2;
				if(end) { /* Necesito que los bytes \r\n sean ajustado a 0 */
					*end = 0;
					*(end + 1) = 0;
				}
				break;
			}
			if(count == MAX_COMPONENTS)
				break;
			tmp = token + 1;
		}
		token++;
	}

	if(*buf == ':') {
		tmp = ret->component[0];
		token = tmp;
		/* Separa component[0] (origen) si tiene el formato *!*@*, consiguiendo nick, user, host */
		while((*token++)) {
			if(*token == '!') {
				*token = 0;
				ret->src_nick = tmp;
				tmp = (token = token + 1);
			} else if(*token == '@') {
				*token = 0;
				ret->src_user = tmp;
				ret->src_host = token + 1;
			}
		}
	}
	ret->count = count;
}

int has_blank(const char *str)
{
	const char *p = str;

	while(*p) {
		if(isblank(*p))
			return 1;
		p++;
	}

	return 0;
}

/* bash read makes sure one line from IRC SERVER */
int main(int c, char **v)
{
	bonix_message msg;
	char buf[BUFSIZE], *format;
	int size, i, order;

	size = read(0, buf, sizeof(buf));
	if(size == -1)
		return RERROR;

	parsing(buf, size, &msg);

	if(msg.count) {

		order = msg.count < 3? 1 : 0;

		printf("%s ", msg.component[order^1]);
		if(msg.src_user)
			printf("\"%s %s %s\" ", msg.src_nick, msg.src_user, msg.src_host);
		else
			printf("%s ", msg.component[order]);

		if(msg.count > 2) {
			printf("%s ", msg.component[2]);

			for(i = 3; i < msg.count; i++) {
				if(has_blank(msg.component[i]))
					format = "\"%s\" ";
				else
					format = "%s ";

				printf(format, msg.component[i]);
			}

			puts("");
		}
	}

	return 0;
}
