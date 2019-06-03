CPP			    = g++
RM			    = rm -f
CPPFLAGS		= -Wall -I. -O2 -std=c++11 -g
LDFLAGS			= -lpthread -lzmqpp -lzmq -lsodium -Wl,--no-as-needed
SOURCES			= ../main_robot.cpp robot.cpp ../utils.cpp ../item.h ../sensors.cpp ../robot_params.cpp ../controllers.cpp ../messages.cpp ../comm_rpi_2.cpp ../../publisher.cpp ../../subscriber.cpp
TARGET			= main_robot

all:	${TARGET}

clean:
	${RM} *.obj *~* ${TARGET}

${TARGET}:
	${CPP} ${CPPFLAGS} -o $@ $(SOURCES) ${LDFLAGS}


