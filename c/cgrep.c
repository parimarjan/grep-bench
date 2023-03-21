#include <string.h>
/*#include <err.h>*/
#include <stdio.h>

char *
fgetln(FILE *fp, size_t *len)
{
	static char *buf = NULL;
	static size_t bufsiz = 0;
	char *ptr;


	if (buf == NULL) {
		bufsiz = BUFSIZ;
		if ((buf = malloc(bufsiz)) == NULL)
			return NULL;
	}

	if (fgets(buf, bufsiz, fp) == NULL)
		return NULL;

	*len = 0;
	while ((ptr = strchr(&buf[*len], '\n')) == NULL) {
		size_t nbufsiz = bufsiz + BUFSIZ;
		char *nbuf = realloc(buf, nbufsiz);

		if (nbuf == NULL) {
			/*int oerrno = errno;*/
			/*free(buf);*/
			/*errno = oerrno;*/
			buf = NULL;
			return NULL;
		} else
			buf = nbuf;

		if (fgets(&buf[bufsiz], BUFSIZ, fp) == NULL) {
			buf[bufsiz] = '\0';
			*len = strlen(buf);
			return buf;
		}

		*len = bufsiz;
		bufsiz = nbufsiz;
	}

	*len = (ptr - buf) + 1;
	return buf;
}

char *strnstr(const char *haystack, const char *needle, size_t len)
{
        int i;
        size_t needle_len;

        if (0 == (needle_len = strnlen(needle, len)))
                return (char *)haystack;

        for (i=0; i<=(int)(len-needle_len); i++)
        {
                if ((haystack[0] == needle[0]) &&
                        (0 == strncmp(haystack, needle, needle_len)))
                        return (char *)haystack;

                haystack++;
        }
        return NULL;
}


int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        fprintf(stderr, "usage: %s TOKEN FILES...\n", argv[0]);
        return 1;
    }

    const char *token = argv[1];

    for(int arg = 2; arg < argc; arg++) {

        const char *path = argv[arg];

        FILE *stream = fopen(path, "r");
        if (stream == NULL) {
            perror(path);
            return 2;
        }

        char * line;
        size_t len;
        while (NULL != (line = fgetln(stream, &len))) {
            char *match = strnstr(line, token, len);
            if (match != NULL) {
                fwrite(line, len, 1, stdout);
            }
        }

        fclose(stream);
    }

    return 0;
}
