//#include "utility/w5100.h"
//#include "utility/socket.h"

extern "C" {
#include "string.h"
#include <stdio.h>
}

#include "Arduino.h"

#include "Ethernet.h"
#include "EthernetClient.h"
#include "EthernetServer.h"
//#include "Dns.h"

uint16_t EthernetClient::_srcport = 49152;      //Use IANA recommended ephemeral port range 49152-65535

EthernetClient::EthernetClient() : _sock(MAX_SOCK_NUM) {
}

EthernetClient::EthernetClient(uint8_t sock) : _sock(sock) {
}

int EthernetClient::connect(const char* host, uint16_t port) {
  // Look up the host first
  int ret = 0;
#if 0
  // DNSClient dns;
  IPAddress remote_addr;

  // dns.begin(Ethernet.dnsServerIP());
  // ret = dns.getHostByName(host, remote_addr);
  if (ret == 1) {
    return connect(remote_addr, port);
  } else {
    return ret;
  }
#endif
  return 0;
}

int EthernetClient::connect(IPAddress ip, uint16_t port) {
  _srcport++;
  if (_srcport == 0) _srcport = 49152;          //Use IANA recommended ephemeral port range 49152-65535

#if 0
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return 0;
  }
  
  memset(&serv_addr, '\0', sizeof(serv_addr));
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr = 0;	// FIX.ME want correct format of ip address
  serv_addr.sin_port = htons(PORT);
      
  
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return 0;
  }
  return 1;
#endif
  return 0;
}

size_t EthernetClient::write(uint8_t b) {
  return write(&b, 1);
}

size_t EthernetClient::write(const uint8_t *buf, size_t size) {
  if (_sock == MAX_SOCK_NUM) {
    setWriteError();
    return 0;
  }
#if 0
  if (!send(_sock, buf, size, 0)) {
    setWriteError();
    return 0;
  }
  return size;
#endif
  return 0;
}

int EthernetClient::available() {
#if 0
  if (_sock != MAX_SOCK_NUM)
    return recvAvailable(_sock);
#endif
  return 0;
}

int EthernetClient::read() {
  uint8_t b;
#if 0
  if ( recv(_sock, &b, 1) > 0 )
  {
    // recv worked
    return b;
  }
  else
  {
    // No data available
    return -1;
  }
#endif
  return -1;
}

int EthernetClient::read(uint8_t *buf, size_t size) {
#if 0
  return recv(_sock, buf, size);
#else
  return 0;
#endif
}

int EthernetClient::peek() {
  uint8_t b;
  // Unlike recv, peek doesn't check to see if there's any data available, so we must
  if (!available())
    return -1;
#if 0
  ::peek(_sock, &b);
  return b;
#endif
  return -1;
}

void EthernetClient::flush() {
#if 0
  ::flush(_sock);
#endif
}

void EthernetClient::stop() {
  if (_sock == MAX_SOCK_NUM)
    return;

#if 0
  // attempt to close the connection gracefully (send a FIN to other side)
  disconnect(_sock);
  unsigned long start = millis();

  // wait up to a second for the connection to close
  uint8_t s;
  do {
    s = status();
    if (s == SnSR::CLOSED)
      break; // exit the loop
    delay(1);
  } while (millis() - start < 1000);
  // if it hasn't closed, close it forcefully
  if (s != SnSR::CLOSED)
    close(_sock);
#endif

  EthernetClass::_server_port[_sock] = 0;
  _sock = MAX_SOCK_NUM;
}

uint8_t EthernetClient::connected() {
#if 0
  if (_sock == MAX_SOCK_NUM) return 0;

  uint8_t s = status();
  return !(s == SnSR::LISTEN || s == SnSR::CLOSED || s == SnSR::FIN_WAIT ||
    (s == SnSR::CLOSE_WAIT && !available()));
#endif
  return 0;
}

uint8_t EthernetClient::status() {
#if 0
  if (_sock == MAX_SOCK_NUM) return SnSR::CLOSED;
  return socketStatus(_sock);
#endif
  return 0;
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

EthernetClient::operator bool() {
  return _sock != MAX_SOCK_NUM;
}

bool EthernetClient::operator==(const EthernetClient& rhs) {
  return _sock == rhs._sock && _sock != MAX_SOCK_NUM && rhs._sock != MAX_SOCK_NUM;
}

uint8_t EthernetClient::getSocketNumber() {
  return _sock;
}
