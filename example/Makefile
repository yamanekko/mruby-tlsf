CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I../mruby/include -I../include
LDFLAGS = -L../mruby/build/host/lib -lmruby
PROGRAM = cmd
OBJS = cmd.o
RAKE = rake
MRUBY_CONFIG = ../build_config_sample.rb
MRUBY_DIR = ../mruby

.PHONY: all mruby

all: mruby $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $^ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

clean:
	$(RM) -f *.o cmd
	cd $(MRUBY_DIR) && $(RAKE) clean

mruby:
	cd $(MRUBY_DIR) && MRUBY_CONFIG=$(MRUBY_CONFIG) $(RAKE) all
