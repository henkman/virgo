
SRCS=virgo.c
OBJS=$(SRCS:.c=.o)
CFLAGS=-O3 -nostdlib -fno-asynchronous-unwind-tables -fno-builtin -fno-ident -ffunction-sections -fdata-sections -Wall
LIBS=-lgdi32 -lmsvcrt -luser32 -lshell32 -lkernel32
LDFLAGS=-static -nostdlib -fno-builtin -s -Wl,-e,__main,--gc-sections,-subsystem,windows $(LIBS)
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
