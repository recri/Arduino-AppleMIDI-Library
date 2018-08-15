
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
  Serial.print("Connected to session ");
  Serial.println(name);
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected = false;
  Serial.println("Disconnected");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOn from channel:");
  Serial.print(channel);
  Serial.print(" note:");
  Serial.print(note);
  Serial.print(" velocity:");
  Serial.print(velocity);
  Serial.println();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Incoming NoteOff from channel:");
  Serial.print(channel);
  Serial.print(" note:");
  Serial.print(note);
  Serial.print(" velocity:");
  Serial.print(velocity);
  Serial.println();
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{

  Serial.print("IP address is ");
  Serial.println(local);
  
  Serial.print("AppleMIDI Session ");
  Serial.print(AppleMIDI.getSessionName());
  Serial.print(" with SSRC 0x");
  Serial.println(AppleMIDI.getSynchronizationSource(), HEX);

  // Create a session and wait for a remote host to connect to us
  AppleMIDI.begin("test");

  Serial.print("OK, now make an active connection to ");
  Serial.println(remote);

  // This is the invite to the remote participant
  AppleMIDI.invite(remote);

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

  AppleMIDI.OnReceiveNoteOn(OnAppleMidiNoteOn);
  AppleMIDI.OnReceiveNoteOff(OnAppleMidiNoteOff);

  Serial.println("Sending NoteOn/Off of note 45, every second");
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
    //   Serial.print(".");

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
