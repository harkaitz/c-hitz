#ifndef HITZ_HITZ_H
#define HITZ_HITZ_H

#include "config.h"
#include <stdlib.h>

typedef struct hitz_command_s hitz_command_t;
typedef struct hitz_module_s  hitz_module_t;
typedef struct hitz_s         hitz_t;
typedef int (*hitz_callback_f) (hitz_t *,hitz_module_t **,const char *[]);
typedef void (*hitz_cleanup_f) (hitz_t *,hitz_module_t *);

     
struct hitz_command_s {
	char		const *name;
	char		const *help;
	hitz_callback_f	callback;
};

struct hitz_module_s {
	char		module[HITZ_MAX_STRING];
	hitz_cleanup_f	cleanup;
	struct		hitz_command_s commands[HITZ_MAX_COMMANDS];
};

struct hitz_s {
	enum {
		HITZ_RUNNING = 0x01,
		HITZ_CLOSED  = 0x02
	} flags;
	void	*udata;
	int	(*split_fs)  (char **argv,int argc,char *buffer);
	int	(*search_nl) (char *buffer,int len);
	void	(*cleanup)   (hitz_t *h);
	int	(*startup)   (hitz_t *h);
	int	(*putc_cb)   (hitz_t *h,int o,char c);
	int	(*getc_cb)   (hitz_t *h);
	int	(*prompt)    (hitz_t *h);
	int	(*match)     (hitz_t *h, hitz_module_t *m, hitz_command_t *c, const char *s[]);
	int	(*err)       (hitz_t *h, const char *e);
	struct	hitz_module_s *modules[HITZ_MAX_MODULES];
	char	reserved[64];
};

/* Initialize and destroy. */

extern void	 hitz_init (hitz_t *h, void *udata,void (*cleanup) (hitz_t *h));
extern void	 hitz_add  (hitz_t *h, hitz_module_t  *m);
extern void	 hitz_del  (hitz_t *h, hitz_module_t **m);
extern void	 hitz_clean(hitz_t *h);
extern void	*hitz_udata(hitz_t *h);

/* Interactive. */
extern char	*hitz_fgets  (hitz_t *h,char *buffer,int len);
extern int	 hitz_fputs  (hitz_t *h,int o,const char *buffer);
extern int	 hitz_fprintf(hitz_t *h,int o,const char *format,...);
extern int	 hitz_run    (hitz_t *h,char *buffer,int len);
extern int	 hitz_exec   (hitz_t *h,const char *argv[]);

/* Threads.    */
extern void	*hitz_loop_and_clean(void *h);

/* Defaults. */
extern int	HITZ_SPLIT_FS(char **,int,char *);
extern int	HITZ_SEARCH_NL(char *,int);
extern int	HITZ_DEFAULT_PUTC(hitz_t *,int,char);
extern int	HITZ_DEFAULT_GETC(hitz_t *);
extern int	HITZ_PROMPT(hitz_t*);
extern int	HITZ_STARTUP(hitz_t*);
extern int	HITZ_MATCH(hitz_t*,hitz_module_t *,hitz_command_t *,const char *[]);
extern int	HITZ_ERR(hitz_t *,const char *);
     
/* Error reporting. */
extern char const	*hitz_strerror(void);
extern int		 hitz_log(int type,const char *format,...);

/* Basic commands. */
extern hitz_module_t HITZ_MODULE;
  
#endif
