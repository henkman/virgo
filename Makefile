CC=gcc
SRCS=virgo.c tray.c
OBJS=$(SRCS:.c=.o)
CFLAGS=-O2 -std=c90 -Wall
LDFLAGS=-s -lgdi32 -mwindows
NAME=virgo
EXE=$(NAME).exe

.PHONY: all clean
all: $(EXE)
$(EXE): $(OBJS) $(NAME).res
	$(CC) -o $(EXE) $(OBJS) $(NAME).res $(LDFLAGS)
	
$(NAME).res: $(NAME).rc
	windres -O coff $(NAME).rc $(NAME).res
	
.c.o:
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(EXE) $(NAME).res
