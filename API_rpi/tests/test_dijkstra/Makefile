CPP			    = g++
RM			    = rm -f
CPPFLAGS		= -Wall -I. -O2 -std=c++11 -g
LDFLAGS			= -lpthread -lzmqpp -lzmq -lsodium -Wl,--no-as-needed
SOURCES			= main_dijkstra.cpp ../../path_planning/dijkstra.cpp ../../graph/graph.cpp ../../graph/edge.cpp ../../graph/node.cpp
TARGET			= dijkstra

all:	${TARGET}

clean:
	${RM} *.obj *~* ${TARGET}

${TARGET}:
	${CPP} ${CPPFLAGS} -o $@ $(SOURCES) ${LDFLAGS}


