#include <Arduino.h>
#define F(x) x

#include <LinuxUdp.h>

#include "AppleMidi.h"

unsigned long t0 = millis();
bool isConnected = false;
IPAddress local;

APPLEMIDI_CREATE_INSTANCE(LinuxUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

// Code size:
//
// IDE 1.8.3
//
// Arduino Ethernet / Uno
// Sketch uses 23976 bytes (74%) of program storage space. Maximum is 32256 bytes.
// Global variables use 969 bytes (47%) of dynamic memory, leaving 1079 bytes for local variables. Maximum is 2048 bytes.

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

  printf("Sending NoteOn/Off of note 45, every second\n");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  // Listen to incoming notes
  AppleMIDI.run();

  // send a note every second
  // (dont cáll delay(1000) as it will stall the pipeline)
  if (isConnected && (millis() - t0) > 1000) {
    t0 = millis();

    byte note = 45;
    byte velocity = 55;
    byte channel = 1;

    AppleMIDI.noteOn(note, velocity, channel);
    AppleMIDI.noteOff(note, velocity, channel);
  }
}

static int streq(const char *p1, const char *p2) { return strcmp(p1,p2)==0; }

int main(int argc, char *argv[]) {
  for (int i = 1; i+1 < argc; i += 2) {
    if (streq(argv[i], "-local"))
      local.fromString(argv[i+1]);
    else {
      printf("unrecognized option %s\n", argv[i]);
      printf("usage: note-on-off-every-second -local localIP\n");
      return 1;
    }
  }
  setup(); while(1) loop(); return 0;
}
