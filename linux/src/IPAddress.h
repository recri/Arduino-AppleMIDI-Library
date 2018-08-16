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
  struct sockaddr_in ip4addr;
  char name[INET_ADDRSTRLEN];
public:
    // Constructors
  IPAddress() { 
    memset((void *)&ip4addr.sin_addr, 0, sizeof(ip4addr.sin_addr));
    inet_ntop(AF_INET, &(ip4addr.sin_addr), name, INET_ADDRSTRLEN);
  }

  const char *toString() {
    return name;
  }

  bool fromString(const char *address)  {
    struct hostent *hp;     /* host information */
    if ( ! (hp = gethostbyname(address))) return 0;
    /* put the host's address into the server address structure */
    memcpy((void *)&ip4addr.sin_addr, hp->h_addr_list[0], hp->h_length);
    inet_ntop(AF_INET, &(ip4addr.sin_addr), name, INET_ADDRSTRLEN);
    return 1;
  }

  bool fromString(const String &address) { return fromString(address.c_str()); }
  // Overloaded cast operator to allow IPAddress objects to be used where a pointer
  // to a four-byte uint8_t array is expected

  struct sockaddr_in sockaddr() { return ip4addr; }
  
  size_t printTo(Print& p) const {
    p.print(name);
    return strlen(name);
  };

  friend class EthernetClass;
  friend class UDP;
  friend class Client;
  friend class Server;
  friend class DhcpClass;
  friend class DNSClient;
};

#endif
