#include "hitz/hitz.h"

int
main(int argc, char *argv[])
{
	hitz_t hitz;
	hitz_init(&hitz,NULL,NULL);
	hitz_add(&hitz,&HITZ_MODULE);
	hitz_loop_and_clean(&hitz);
}

