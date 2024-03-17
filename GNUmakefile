PROJECT=hitz
VERSION=1.0.0
PREFIX=/usr/local
PROGS=
CC=gcc -Wall -std=c99
SOURCES=hitz/hitz-commands.c hitz/hitz-stdio.c hitz/hitz.c
HEADERS=hitz/hitz.h hitz/config.h

all: test_hitz libhitz.a
clean:
	rm -f $(SOURCES:.c=.o) libhitz.a test_hitz
install:
	mkdir -p $(DESTDIR)$(PREFIX)/include/hitz $(DESTDIR)$(PREFIX)/lib
	cp hitz/*.h $(DESTDIR)$(PREFIX)/include/hitz
	cp libhitz.a $(DESTDIR)$(PREFIX)/lib
##
test_hitz: example.c libhitz.a
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ example.c libhitz.a
libhitz.a: $(SOURCES:.c=.o)
	ar rcs libhitz.a $(SOURCES:.c=.o)
##
.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
