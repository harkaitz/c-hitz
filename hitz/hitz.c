#include "hitz.h"
#include <string.h>
#include <stdarg.h>
#include <strings.h>
#include <stdio.h>

static __thread char hitz_errstr[512] = {0};

void
hitz_init(hitz_t *h, void *udata, void (*cleanup) (hitz_t *h))
{
	memset(h, 0, sizeof(*h));
	h->flags     = HITZ_RUNNING;
	h->udata     = udata;
	h->split_fs  = HITZ_SPLIT_FS;
	h->search_nl = HITZ_SEARCH_NL;
	h->cleanup   = cleanup;
	h->startup   = HITZ_STARTUP;
	h->putc_cb   = HITZ_DEFAULT_PUTC;
	h->getc_cb   = HITZ_DEFAULT_GETC;
	h->prompt    = HITZ_PROMPT;
	h->match     = HITZ_MATCH;
	h->err       = HITZ_ERR;
}

void
hitz_add(hitz_t *h, hitz_module_t *m)
{
	for (int i=0; i<HITZ_MAX_MODULES; i++) {
		if (!h->modules[i]) {
			h->modules[i] = m;
			break;
		}
	}
}

void
hitz_del(hitz_t *h, hitz_module_t **m)
{
	if(!m) return;
	if((*m)->cleanup) (*m)->cleanup(h,*m);
	*m = NULL;
}

void
hitz_clean(hitz_t *h)
{
	if(h->cleanup) {
		h->cleanup(h);
	}
}

void *
hitz_udata(hitz_t *h)
{
	return h->udata;
}

char *
hitz_fgets(hitz_t *h,char *buffer,int len)
{
	int f = -1;
	if (h->flags & HITZ_CLOSED) {
		return NULL;
	}
	h->prompt(h);
	for (int i=0; i<len; i++) {
		buffer[i] = h->getc_cb(h);
		if(buffer[i]==EOF) {
			break;
		}
		f = h->search_nl(buffer,i+1);
		if(f>=0) {
			break;
		}
	}
	if (f>=0) {
		buffer[f] = '\0';
		return buffer;
	} else {
		return NULL;
	}
}

int
hitz_fputs(hitz_t *h, int o, const char *buffer)
{
	int i;
	for(i=0; buffer[i]; i++) {
		h->putc_cb(h, o, buffer[i]);
	}
	return i;
}

int
hitz_fprintf(hitz_t *h,int o,const char *format,...)
{
	char buffer[1024*5];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer,sizeof(buffer),format,ap);
	va_end(ap);
	return hitz_fputs(h,o,buffer);
}

int
hitz_run(hitz_t *h,char *buffer,int len)
{
	char *args[20];
	h->split_fs(args,20,buffer);
	return hitz_exec(h,(void*)args);
}

int
hitz_exec(hitz_t *h,const char *argv[])
{
	if(!argv[0]) {
		return 0;
	}
	for(int i=0; i<HITZ_MAX_MODULES; i++) {
		hitz_module_t *m = h->modules[i];
		if(!m) {
			continue;
		}
		for(int j=0; m->commands[j].name && m->commands[j].callback; j++) {
			hitz_command_t *c = &m->commands[j];
			if(h->match(h,m,c,argv)) {
				return c->callback(h,&(h->modules[i]),argv);
			}
		}
	}
	hitz_log(-1,"Command `%s` not found.",argv[0]);
	return h->err(h,hitz_strerror());
}

void *
hitz_loop_and_clean(void *u)
{
	char buffer[514] = {0};
	hitz_t *h = (hitz_t*) u;
	if(h->startup(h)<0) {
		return NULL;
	}
	for(char *s;(s=hitz_fgets(h,buffer,sizeof(buffer)-1));) {
		int r = hitz_run(h,buffer,strlen(buffer));
		if(r<-1) break;
	}
	hitz_clean(h);
	return NULL;
}

const char *
hitz_strerror(void)
{
	return hitz_errstr;
}

int
hitz_log(int type,const char *format,...)
{
	va_list args;
	va_start(args,format);
	if(type > 0) {
		if(getenv("hitz_DEBUG")) {
			fprintf(stderr,"hitz: [D] ");
			vfprintf(stderr,format, args);
			fprintf(stderr,"\n");
		}
	} else if(type == 0) {
		fprintf(stderr,"hitz: [I] ");
		vfprintf(stderr,format, args);
		fprintf(stderr,"\n");
	} else if(type < 0) {
		vsnprintf (hitz_errstr,sizeof(hitz_errstr)-1,format, args);
		if(getenv("hitz_DEBUG")) {
			fprintf(stderr,"hitz: [E] %s\n",hitz_errstr);
		}
	}
	va_end(args);
	return type;
}
