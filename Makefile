CC=gcc
SRCS=virgo.c tray.c
OBJS=$(SRCS:.c=.o)
CFLAGS=-O2 -std=c99 -Wall 
LDFLAGS=-s -lgdi32 -mwindows
ARCH=32
ifeq ($(ARCH), 64)
	WINDRES_ARCH=pe-x86-64
else
	WINDRES_ARCH=pe-i386
endif
NAME=virgo
EXE=$(NAME).exe

.PHONY: all clean
all: $(EXE)
$(EXE): $(OBJS) $(NAME).res
	$(CC) -o $(EXE) $(OBJS) $(NAME).res -m$(ARCH) $(LDFLAGS)
	
$(NAME).res: $(NAME).rc
	windres -O coff -F $(WINDRES_ARCH) $(NAME).rc $(NAME).res 
	
.c.o:
	$(CC) -o $@ $(CFLAGS) -m$(ARCH) -c $<

clean:
	rm -f $(OBJS) $(EXE) $(NAME).res
