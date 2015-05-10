# Straight forward Makefile to compile all examples in a row

# If cross-compiling, you may wish to set the following environment variable to the root location of 
# your 'sdk'
# SDKSTAGE=

INCDIR=-I../Libs -I$(SDKSTAGE)/opt/vc/include -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux

LIBS=-lGLESv2 -lEGL -lm -lbcm_host -lwiringPi -L$(SDKSTAGE)/opt/vc/lib

# CFLAGS+=-DRPI_NO_X

CFLAGS+=-DRPI_NO_X -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi -I$(SDKSTAGE)/opt/vc/include/ -I./ -I$(SDKSTAGE)/opt/vc/lib



COMMONSRC=../Libs/esShader.c    \
          ../Libs/esTransform.c \
          ../Libs/esShapes.c    \
          ../Libs/esUtil.c      \
          ../Libs/rpUtil.c      \
          ../Libs/rpNote.c      \
          ../Libs/kshLoader.c
COMMONHRD=../Libs/esUtil.h      \
          ../Libs/rpUtil.h      \
          ../Libs/rpNote.h      \
          ../Libs/kshLoader.h

CH02SRC=main.c

KSHLOAD=testKshLoader.c

default: all

all: main testKshLoader

clean:
	find . -name "CH??_*" | xargs rm -f

main: ${COMMONSRC} ${COMMONHDR} ${CH02SRC}
	gcc $(CFLAGS) ${COMMONSRC} ${CH02SRC} -o ./$@ ${INCDIR} ${LIBS}

testKshLoader: ${COMMONSRC} ${COMMONHDR} ${KSHLOAD}
	gcc $(CFLAGS) ${COMMONSRC} ${KSHLOAD} -o ./$@ ${INCDIR} ${LIBS}
