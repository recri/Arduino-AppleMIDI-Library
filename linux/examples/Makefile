INCLUDE=-I../src -I../../src
DEFINE=-DLINUX=1	
all: initiate-session

initiate-session: initiate-session.cpp ../src/*.cpp ../src/*.c
	g++ -o initiate-session $(DEFINE) $(INCLUDE) initiate-session.cpp ../src/*.cpp ../src/*.c
