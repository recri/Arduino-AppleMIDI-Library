#include <Arduino.h>

#include <LinuxUdp.h>

#include "AppleMidi.h"

unsigned long t0 = millis();
bool isConnected = false;
IPAddress local;

APPLEMIDI_CREATE_INSTANCE(LinuxUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

// ====================================================================================
// Event handlers for incoming MIDI messages
// ====================================================================================

// -----------------------------------------------------------------------------
// rtpMIDI session. Device connected
// -----------------------------------------------------------------------------
void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  isConnected = true;
  printf("Connected to session %s\n", name);
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected = false;
  printf("Disconnected\n");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
  printf("Incoming NoteOn from channel: %d note: %d velocity: %d\n", channel, note, velocity);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
  printf("Incoming NoteOff from channel: %d note: %d velocity: %d\n", channel, note, velocity);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiClock() { printf("Clock\n"); }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiStart() { printf("Start\n"); }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiStop() { printf("Stop\n"); }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiContinue() { printf("Continue\n"); }

// -----------------------------------------------------------------------------
// (https://www.midi.org/specifications/item/table-1-summary-of-midi-message)
// Active Sensing. 
// This message is intended to be sent repeatedly to tell the receiver that a 
// connection is alive. Use of this message is optional. When initially received, 
// the receiver will expect to receive another Active Sensing message each 300ms (max), 
// and if it does not then it will assume that the connection has been terminated. 
// At termination, the receiver will turn off all voices and return to normal 
// (non- active sensing) operation. 
// -----------------------------------------------------------------------------
static void OnAppleMidiActiveSensing() {
  printf("ActiveSensing\n");
  AppleMIDI.activeSensing();
}

// -----------------------------------------------------------------------------
// (https://www.midi.org/specifications/item/table-1-summary-of-midi-message)
// Reset. 
// Reset all receivers in the system to power-up status. This should be used 
// sparingly, preferably under manual control. In particular, it should not be 
// sent on power-up.
// -----------------------------------------------------------------------------
static void OnAppleMidiReset() {
  printf("Reset\n");
}

// -----------------------------------------------------------------------------
// (https://www.midi.org/specifications/item/table-1-summary-of-midi-message)
// Song Position Pointer. 
// This is an internal 14 bit register that holds the number of MIDI beats 
// (1 beat= six MIDI clocks) since the start of the song. l is the LSB, m the MSB.
// -----------------------------------------------------------------------------
static void OnAppleMidiSongPosition(unsigned short a) {
  printf("SongPosition: %d\n", a);
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  printf("OK, now make sure you have an rtpMIDI session that is Enabled\n");
  printf("Add device named Linux with Host/Port %s:5004\n", local.toString());
  printf("Then press the Connect button\n");
  printf("Then open a MIDI listener (eg MIDI-OX) and monitor incoming notes\n");

  // Create a session and wait for a remote host to connect to us
  AppleMIDI.begin("test");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

  AppleMIDI.OnReceiveNoteOn(OnAppleMidiNoteOn);
  AppleMIDI.OnReceiveNoteOff(OnAppleMidiNoteOff);

  AppleMIDI.OnReceiveClock(OnAppleMidiClock);
  AppleMIDI.OnReceiveStart(OnAppleMidiStart);
  AppleMIDI.OnReceiveStop(OnAppleMidiStop);
  AppleMIDI.OnReceiveContinue(OnAppleMidiContinue);
  AppleMIDI.OnReceiveActiveSensing(OnAppleMidiActiveSensing);
  AppleMIDI.OnReceiveReset(OnAppleMidiReset);

  AppleMIDI.OnReceiveSongPosition(OnAppleMidiSongPosition);

  printf("Listening for Clock events\n");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  // Listen to incoming notes
  AppleMIDI.run();
}

static int streq(const char *p1, const char *p2) { return strcmp(p1,p2)==0; }

int main(int argc, char *argv[]) { 
  for (int i = 1; i+1 < argc; i += 2) {
    if (streq(argv[i], "-local"))
      local.fromString(argv[i+1]);
    else {
      printf("unrecognized option %s\n", argv[i]);
      printf("usage: realtime-messages -local localIP\n");
      return 1;
    }
  }
  setup(); while(1) loop(); return 0;
}
