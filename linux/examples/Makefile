INCLUDE=-I../src -I../../src
DEFINE=-DLINUX=1	
all: initiate-session initiate-sessions note-on-off-every-second realtime-messages receive-MTC sysex

initiate-session: initiate-session.cpp ../src/*.cpp  ../src/*.h
	g++ -o initiate-session $(DEFINE) $(INCLUDE) initiate-session.cpp ../src/*.cpp 

initiate-sessions: initiate-sessions.cpp ../src/*.cpp ../src/*.h
	g++ -o initiate-sessions $(DEFINE) $(INCLUDE) initiate-sessions.cpp ../src/*.cpp 

note-on-off-every-second: note-on-off-every-second.cpp ../src/*.cpp ../src/*.h
	g++ -o note-on-off-every-second $(DEFINE) $(INCLUDE) note-on-off-every-second.cpp ../src/*.cpp 

realtime-messages: realtime-messages.cpp ../src/*.cpp ../src/*.h
	g++ -o realtime-messages $(DEFINE) $(INCLUDE) realtime-messages.cpp ../src/*.cpp 

receive-MTC: receive-MTC.cpp ../src/*.cpp ../src/*.h
	g++ -o receive-MTC $(DEFINE) $(INCLUDE) receive-MTC.cpp ../src/*.cpp 

sysex: sysex.cpp ../src/*.cpp ../src/*.h
	g++ -o sysex $(DEFINE) $(INCLUDE) sysex.cpp ../src/*.cpp 
