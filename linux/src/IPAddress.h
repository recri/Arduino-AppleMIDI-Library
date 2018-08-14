/*
  IPAddress.h - Base class that provides IPAddress
  Copyright (c) 2011 Adrian McEwen.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef IPAddress_h
#define IPAddress_h

#include <Arduino.h>
#include "Printable.h"
#include "WString.h"

// A class to make it easier to handle and pass around IP addresses

// this is all ipv6 toxic, doesn't know such addresses exist

class IPAddress : public Printable {
private:
  union {  // these are currently being kept in presentation order
    uint8_t bytes[4];  // IPv4 address
    uint32_t dword;
  } _address;
  
  // Access the raw byte array containing the address.  Because this returns a pointer
  // to the internal structure rather than a copy of the address this function should only
  // be used when you know that the usage of the returned uint8_t* will be transient and not
  // stored.
  uint8_t* raw_address() { return _address.bytes; };
public:
    // Constructors
  IPAddress() { 
    _address.dword = 0; 
  }

  IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet) {
    _address.bytes[0] = first_octet;
    _address.bytes[1] = second_octet;
    _address.bytes[2] = third_octet;
    _address.bytes[3] = fourth_octet;
  }

  IPAddress(uint32_t address) { 
    _address.dword = address;
  }

  IPAddress(const uint8_t *address) { 
    memcpy(_address.bytes, address, sizeof(_address.bytes)); 
  }

  bool fromString(const char *address)  {
    struct hostent *hp;     /* host information */
    struct in_addr addr;
    if ( ! (hp = gethostbyname(address))) return 0;
    /* put the host's address into the server address structure */
    memcpy((void *)&addr, hp->h_addr_list[0], hp->h_length);
    _address.dword = ntohl(addr.s_addr);
    return 1;
  }

  bool fromString(const String &address) { return fromString(address.c_str()); }
  // Overloaded cast operator to allow IPAddress objects to be used where a pointer
  // to a four-byte uint8_t array is expected
  operator uint32_t() const { return _address.dword; };
  bool operator==(const IPAddress& addr) const { return _address.dword == addr._address.dword; };
  bool operator==(const uint8_t* addr) const {
    return memcmp(addr, _address.bytes, sizeof(_address.bytes)) == 0;
  }

  // Overloaded index operator to allow getting and setting individual octets of the address
  uint8_t operator[](int index) const { return _address.bytes[index]; }
  uint8_t& operator[](int index) { return _address.bytes[index]; }
  
  // Overloaded copy operators to allow initialisation of IPAddress objects from other types
  IPAddress& operator=(const uint8_t *address) {
    memcpy(_address.bytes, address, sizeof(_address.bytes));
    return *this;
  }
  IPAddress& operator=(uint32_t address) {
    _address.dword = address;
    return *this;
  }

  size_t printTo(Print& p) const {
    size_t n = 0;
    for (int i =0; i < 3; i++)
      {
	n += p.print(_address.bytes[i], DEC);
	n += p.print('.');
      }
    n += p.print(_address.bytes[3], DEC);
    return n;
  };

  friend class EthernetClass;
  friend class UDP;
  friend class Client;
  friend class Server;
  friend class DhcpClass;
  friend class DNSClient;
};

const IPAddress xINADDR_NONE(0,0,0,0);

#endif
