#! make
CC=gcc
CFLAGS=-c -Wall -std=c99 -I./src/tests -I./src/lib 
LDFLAGS= 
MAKE=make
SOURCES=main.c replace_bbcode.c test_bbcode.c
EXECUTABLE=testbbcode.exe
BUILDDIR=./bin/debug/

#LINT=lint -p
OBJECTS=$(addprefix $(BUILDDIR),$(SOURCES:.c=.o))
#OBJECTS=main.o replace_bbcode.o test_bbcode.o

EXECUTABLEPATH=$(BUILDDIR)$(EXECUTABLE)

all: $(EXECUTABLEPATH) 

folder:
	mkdir -p $(BUILDDIR)

$(EXECUTABLEPATH): $(OBJECTS) folder
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

run: $(EXECUTABLEPATH)
	cd $(BUILDDIR); ./$(EXECUTABLE);

clean: 
	rm $(OBJECTS) 
	rm $(EXECUTABLEPATH)
	rmdir $(BUILDDIR)

$(BUILDDIR)main.o: ./src/tests/main.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)replace_bbcode.o: ./src/lib/replace_bbcode.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)test_bbcode.o: ./src/tests/test_bbcode.c
	$(CC) $(CFLAGS) -c $< -o $@

#lint: $(SOURCES)
#	$(LINT) $(SOURCES)

$(BUILDDIR)%.o: %.c	folder
	$(CC) $(CFLAGS) -c $< -o $@
