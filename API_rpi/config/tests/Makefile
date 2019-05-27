CPP			    = g++
RM			    = rm -f
CPPFLAGS		= -Wall -I. -O2 -std=c++11 -g
LDFLAGS			= -lpthread -lzmqpp -lzmq -lsodium -Wl,--no-as-needed
SOURCES			= tests.cpp ../config.cpp ../../robot/utils.cpp
TARGET			= tests

all:	${TARGET}

clean:
	${RM} *.obj *~* ${TARGET}

${TARGET}:
	${CPP} ${CPPFLAGS} -o $@ $(SOURCES) ${LDFLAGS}


