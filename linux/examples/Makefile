DEFINE=-DLINUX=1 -DAPPLEMIDI_DEBUG=1 -DDEBUG_BUILD=1
INCLUDE=-I../src -I../../src
CFLAGS=-g $(DEFINE) $(INCLUDE) 
all: initiate-session initiate-sessions note-on-off-every-second realtime-messages receive-MTC sysex

clean:
	rm initiate-session initiate-sessions note-on-off-every-second realtime-messages receive-MTC sysex

initiate-session: initiate-session.cpp ../src/*.cpp  ../src/*.h
	g++ -o initiate-session $(CFLAGS) initiate-session.cpp ../src/*.cpp 

initiate-sessions: initiate-sessions.cpp ../src/*.cpp ../src/*.h
	g++ -o initiate-sessions $(CFLAGS) initiate-sessions.cpp ../src/*.cpp 

note-on-off-every-second: note-on-off-every-second.cpp ../src/*.cpp ../src/*.h
	g++ -o note-on-off-every-second $(CFLAGS) note-on-off-every-second.cpp ../src/*.cpp 

realtime-messages: realtime-messages.cpp ../src/*.cpp ../src/*.h
	g++ -o realtime-messages $(CFLAGS) realtime-messages.cpp ../src/*.cpp 

receive-MTC: receive-MTC.cpp ../src/*.cpp ../src/*.h
	g++ -o receive-MTC $(CFLAGS) receive-MTC.cpp ../src/*.cpp 

sysex: sysex.cpp ../src/*.cpp ../src/*.h
	g++ -o sysex $(CFLAGS) sysex.cpp ../src/*.cpp 
