INCLUDE=-I../src -I../../src
DEFINE=-DLINUX=1	
all: initiate-session initiate-sessions note-on-off-every-second realtime-messages

initiate-session: initiate-session.cpp ../src/*.cpp 
	g++ -o initiate-session $(DEFINE) $(INCLUDE) initiate-session.cpp ../src/*.cpp 

initiate-sessions: initiate-sessions.cpp ../src/*.cpp 
	g++ -o initiate-sessions $(DEFINE) $(INCLUDE) initiate-sessions.cpp ../src/*.cpp 

note-on-off-every-second: note-on-off-every-second.cpp ../src/*.cpp 
	g++ -o note-on-off-every-second $(DEFINE) $(INCLUDE) note-on-off-every-second.cpp ../src/*.cpp 

realtime-messages: realtime-messages.cpp ../src/*.cpp 
	g++ -o realtime-messages $(DEFINE) $(INCLUDE) realtime-messages.cpp ../src/*.cpp 
