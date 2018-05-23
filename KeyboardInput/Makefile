INCDIR=-I../Libs
LIBS=-L/opt/vc/lib -lncurses

COMMONSRC=../Libs/rpUtil.c
COMMONHDR=../Libs/rpUtil.h

SRC=kin.c
SRC02=keyEvent.c

default: all

all: kin keyEvent

kin: $(COMMONSRC) $(COMMONHDR) $(SRC)
	gcc $(COMMONSRC) $(SRC) -o ./$@ $(INCDIR) $(LIBS)

keyEvent: $(COMMONSRC) $(COMMONHDR) $(SRC)
	gcc $(COMMONSRC) $(SRC02) -o ./$@ $(INCDIR) $(LIBS)
