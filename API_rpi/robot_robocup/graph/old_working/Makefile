CPP			    = g++
RM			    = rm -f
CPPFLAGS		= -Wall -I. -O2 -std=c++11 -g
LDFLAGS			= -lpthread -lzmqpp -lzmq -lsodium -Wl,--no-as-needed
SOURCES			= old_dijkstra.cpp
TARGET			= old_dijkstra

all:	${TARGET}

clean:
	${RM} *.obj *~* ${TARGET}

${TARGET}:
	${CPP} ${CPPFLAGS} -o $@ $(SOURCES) ${LDFLAGS}


