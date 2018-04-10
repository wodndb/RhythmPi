INCDIR=-I../Libs
LIBS=-L/opt/vc/lib

COMMONSRC=../Libs/rpUtil.c
COMMONHDR=../Libs/rpUtil.h

SRC=kin.c

default: all

all: kin

kin: $(COMMONSRC) $(COMMONHDR) $(SRC)
	gcc $(COMMONSRC) $(SRC) -o ./$@ $(INCDIR) $(LIBS)
