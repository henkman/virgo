CC=gcc
SRCS=virgo.c tray.c
OBJS=$(SRCS:.c=.o)
CFLAGS=-O2
LDFLAGS=-s -lgdi32 -mwindows
NAME=virgo
EXE=$(NAME).exe

.PHONY: all clean
all: $(EXE)
$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LDFLAGS)
	
.c.o:
	$(CC) -Wall -o $@ $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(EXE)
