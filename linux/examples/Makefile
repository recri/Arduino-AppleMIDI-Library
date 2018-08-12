INCLUDE=-I../src -I../../src
DEFINE=-DLINUX=1	
all: initiate-session

initiate-session: initiate-session.cpp ../src/*.cpp
	g++ -o initiate-session $(DEFINE) $(INCLUDE) initiate-session.cpp ../src/*.cpp
