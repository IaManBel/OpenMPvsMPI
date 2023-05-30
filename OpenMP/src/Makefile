GCC = gcc 
oT = -fopenmp -O3 

CFLAGS = -lm 

oL= ../sample/Otime.c

BINDIR = ../BIN/
PROGS = $(BINDIR)MM1f $(BINDIR)MM1c $(BINDIR)MM2f $(BINDIR)MM1fu 

all: MM1f MM1fu MM1c MM2f MM2fblk

clean:
	$(RM) $(PROGS)

MM1c:
	$(GCC) $(oT) $(oL) $@.c -o $(BINDIR)$@ $(CFLAGS)
MM1f:
	$(GCC) $(oT) $(oL) $@.c -o $(BINDIR)$@
MM1fu:
	$(GCC) $(oT) $(oL) $@.c -o $(BINDIR)$@
MM2f:
	$(GCC) $(oT) $(oL) $@.c -o $(BINDIR)$@
MM2fblk:
	$(GCC) $(oT) $(oL) $@.c -o $(BINDIR)$@
