#include <Arduino.h>
#define F(x) x

#include <LinuxUdp.h>

#include "AppleMidi.h"

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
  Serial.print(F("Connected to session "));
  Serial.println(name);
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected = false;
  Serial.println(F("Disconnected"));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
static void OnAppleMidiTimeCodeQuarterFrame(byte data) {
  Serial.print("MTC: ");
  Serial.println(data, HEX);
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  // Serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println(F("OK, now make sure you an rtpMIDI session that is Enabled"));
  Serial.print(F("Add device named Arduino with Host/Port "));
  Serial.print(local);
  Serial.println(F(":5004"));
  Serial.println(F("Then press the Connect button"));
  Serial.println(F("Then open MIDI-OX, ,<View> <MTC Transport>. Then hit play button and see the MTC messages"));

  // Create a session and wait for a remote host to connect to us
  AppleMIDI.begin("test");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

  AppleMIDI.OnReceiveTimeCodeQuarterFrame(OnAppleMidiTimeCodeQuarterFrame);
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
      printf("usage: initiate-sessions -local localIP\n");
      return 1;
    }
  }
  setup(); while(1) loop(); return 0;
}
