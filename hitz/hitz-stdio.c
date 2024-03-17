#ifdef __unix__
#  define _POSIX_C_SOURCE 200809L
#endif
#include "hitz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#ifdef _WIN32
#  include <windows.h>
#  include <io.h>
#  define fp_isatty(FP) _isatty(_fileno(FP))
#else
#  include <unistd.h>
#  define fp_isatty(FP) isatty(fileno(FP))
#endif

int
HITZ_SPLIT_FS(char **argv, int argc, char *buffer)
{
	int i;
	for(i=0, argv[i]=strtok(buffer," \t");
	    argv[i] && i<(argc-1);
	    argv[++i]=strtok(NULL," \t")) {}
	argv[i]=NULL;
	return i;
}

int
HITZ_SEARCH_NL(char *buffer,int len)
{
	for(int i=0; i<len; i++) {
		if(!buffer[i]) break;
		if(buffer[i]=='\n') return i;
	}
	return -1;
}

int
HITZ_DEFAULT_PUTC(hitz_t *h,int o,char c)
{
	switch(o) {
	case 1: return fputc(c,stdout);
	case 2: return fputc(c,stderr);
	case 3:
		if(fp_isatty(stdin)) {
			fputc(c,stderr); fflush(stderr);
		}
		break;
	}
	return -1;
}

int
HITZ_DEFAULT_GETC(hitz_t *h)
{
	return fgetc(stdin);
}

int
HITZ_PROMPT(hitz_t *h)
{
	return hitz_fputs(h,3,"> ");
}

int
HITZ_STARTUP(hitz_t *h)
{
	return 0;
}

int
HITZ_MATCH(hitz_t *h,hitz_module_t *m,hitz_command_t *c,const char *s[])
{
	return (!strcasecmp(c->name,s[0]));
}

int
HITZ_ERR(hitz_t *h,const char *e)
{
	hitz_fputs(h,2,"Error: ");
	hitz_fputs(h,2,e);
	hitz_fputs(h,2,"\n");
	return -1;
}
