
#include <Arduino.h>

#include <LinuxUdp.h>

#include "AppleMidi.h"

unsigned long t0 = millis();
bool isConnected = false;

APPLEMIDI_CREATE_INSTANCE(LinuxUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

IPAddress local;		// fill in local ip with command line option
IPAddress remote;		// fill in remote ip with command line option

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
void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
  printf("Incoming NoteOn from channel: %d note: %d velocity: %d\n", channel, note, velocity);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
  printf("Incoming NoteOff from channel: %d note: %d velocity: %d\n", channel, note, velocity);
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  printf("local IP address is %s\n", local.toString());
  
  printf("AppleMIDI Session %s with SSRC 0x%x\n", AppleMIDI.getSessionName(), AppleMIDI.getSynchronizationSource());

  // Create a session and wait for a remote host to connect to us
  AppleMIDI.begin("test");

  printf("OK, now making an active connection to %s\n", remote.toString());

  // This is the invite to the remote participant
  AppleMIDI.invite(remote);

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
  if (isConnected && (millis() - t0) > 1000)
  {
    t0 = millis();

    int note = 45;
    int velocity = 55;
    int channel = 1;

    AppleMIDI.noteOn(note, velocity, channel);
    AppleMIDI.noteOff(note, velocity, channel);
  }
}

static int streq(const char *p1, const char *p2) { return strcmp(p1,p2)==0; }

int main(int argc,  const char *argv[]) {
  for (int i = 1; i+1 < argc; i += 2) {
    if (streq(argv[i], "-local"))
      local.fromString(argv[i+1]);
    else if (streq(argv[i], "-remote"))
      remote.fromString(argv[i+1]);
    else {
      printf("unrecognized option %s\n", argv[i]);
      printf("usage: initiate-session -local localIP -remote remoteIP\n");
      return 1;
    }
  }
  setup(); while (1) loop(); return 0;
}
