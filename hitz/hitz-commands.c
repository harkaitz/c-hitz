#include "hitz.h"

int
HITZ_COMMAND_HELP(hitz_t *h,hitz_module_t **module,const char *argv[])
{
	hitz_fputs(h,3,"===========================================\n");
	for(int i=0; i<HITZ_MAX_MODULES; i++) {
		hitz_module_t *m = h->modules[i];
		if(!m) {
			continue;
		}
		hitz_fprintf(h,3,"Module: %s\n",m->module);
		for(int j=0; m->commands[j].name && m->commands[j].callback; j++) {
			hitz_command_t *c = &m->commands[j];
			if(!c->help) {
				continue;
			}
			hitz_fprintf(h,3,"+ %-20s : %s\n",c->name,c->help);
		}
	}
	hitz_fputs(h,3,"===========================================\n");
	return 0;
}

int
HITZ_COMMAND_QUIT(hitz_t *h,hitz_module_t **module,const char *argv[])
{
	h->flags |= HITZ_CLOSED;
	return 0;
}

hitz_module_t HITZ_MODULE = {
     "hitz",NULL,{
	  {"help","Print this help.",HITZ_COMMAND_HELP},
	  {"quit","Quit program."   ,HITZ_COMMAND_QUIT},
	  {"exit","Quit program."   ,HITZ_COMMAND_QUIT},
	  {NULL,NULL,NULL}
     }
};
