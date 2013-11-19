CFLAGS=-W -Wall -Werror -pedantic

EXE=main
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)


.PHONY: all clean mrproper

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c ../minunit.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) *.o *~

mrproper: clean
	$(RM) $(EXE)
