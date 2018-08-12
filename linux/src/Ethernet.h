#ifndef ethernet_h
#define ethernet_h

#include <inttypes.h>
//#include "w5100.h"
#include "IPAddress.h"
#include "EthernetClient.h"
#include "EthernetServer.h"
//#include "Dhcp.h"

#define MAX_SOCK_NUM 4

class EthernetClass {
private:
  IPAddress _localIP;
  IPAddress _subnetMask;
  IPAddress _gatewayIP;
  IPAddress _dnsServerIP;
public:
  static uint8_t _state[MAX_SOCK_NUM];
  static uint16_t _server_port[MAX_SOCK_NUM];

  int begin() { return 1; };
  int maintain() { return 1; };

  IPAddress localIP() { return _localIP; };
  IPAddress subnetMask() { return _subnetMask; };
  IPAddress gatewayIP() { return _gatewayIP; };
  IPAddress dnsServerIP() { return _dnsServerIP; };

  friend class EthernetClient;
  friend class EthernetServer;
};

extern EthernetClass Ethernet;

#endif
