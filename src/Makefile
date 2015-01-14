IDIR =../include
CC=clang
CFLAGS= -w -g -I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = data.h base64.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o base64.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	clang -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 