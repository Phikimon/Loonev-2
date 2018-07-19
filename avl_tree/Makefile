CC=gcc
LD=gcc
CFLAGS=-O3 -g -Wall -pedantic -Wno-format -fprofile-arcs -ftest-coverage
LFLAGS= -fprofile-arcs -ftest-coverage
LIBS=-lm
DEPS=avlt.h avln.h common.h

ODIR=.
_OBJ = avlt.o avln.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: tags unit_test

tags:
	ctags -R

$(ODIR):
	mkdir $(ODIR)

$(ODIR)/%.o: %.c $(ODIR) $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

unit_test: $(OBJ)
	$(LD) $(LFLAGS) $(LIBS) $^ -o unit_test

clean:
	rm -rf $(ODIR)/*.o $(ODIR)/*.gcno $(ODIR)/*.gcov $(ODIR)/*.gcda
	rm -f unit_test treeDump* tags
