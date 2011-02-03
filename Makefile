#! make
CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS= 
MAKE=make
#SOURCES=*.c
SOURCES=main.c replace_bbcode.c test_bbcode.c
EXECUTABLE=testbbcode.exe
BUILDDIR=./bin/debug/

#LINT=lint -p
OBJECTS=$(addprefix $(BUILDDIR),$(SOURCES:.c=.o))

EXECUTABLEPATH=$(BUILDDIR)$(EXECUTABLE)

all: $(SOURCES) $(EXECUTABLEPATH) 

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

#lint: $(SOURCES)
#	$(LINT) $(SOURCES)

$(BUILDDIR)%.o: %.c	folder
	$(CC) $(CFLAGS) -c $< -o $@
