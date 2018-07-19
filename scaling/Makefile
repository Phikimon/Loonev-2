CC=gcc
LD=gcc
CFLAGS=-O0 -Wall -pedantic -Wno-format -pthread
LIBS=-lm -pthread
DEPS=

ODIR=obj
_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: tags logarithm

tags:
	ctags -R

$(ODIR):
	mkdir $(ODIR)

$(ODIR)/%.o: %.c $(ODIR) $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

logarithm: $(OBJ)
	$(LD) $(LFLAGS) $(LIBS) $^ -o logarithm

clean:
	rm -rf $(ODIR)/*.o logarithm
	rm -rf obj
	rm tags

