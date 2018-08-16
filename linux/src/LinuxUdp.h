/*
 * NOTE: UDP is fast, but has some important limitations (thanks to Warren Gray for mentioning these)
 * 1) UDP does not guarantee the order in which assembled UDP packets are received. This
 * might not happen often in practice, but in larger network topologies, a UDP
 * packet can be received out of sequence. 
 * 2) UDP does not guard against lost packets - so packets *can* disappear without the sender being
 * aware of it. Again, this may not be a concern in practice on small local networks.
 * For more information, see http://www.cafeaulait.org/course/week12/35.html
 *
 * MIT License:
 * Copyright (c) 2008 Bjoern Hartmann
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * bjoern@cs.stanford.edu 12/30/2008
 */

#ifndef linuxudp_h
#define linuxudp_h

extern "C" {
#define _BUFFER_SIZE 1024
}

#include <Arduino.h>
#include <Udp.h>

class LinuxUDP : public UDP {
private:
  uint16_t _port;		// local port to listen on
  int16_t _sock;		// file descriptor for socket

  IPAddress _remoteIP;		// remote IP address for the incoming packet whilst it's being processed
  uint16_t _remotePort;		// remote port for the incoming packet whilst it's being processed
  int _ioffset;		// offset into input buffer
  int _ilimit;		/* number of bytes received */
  uint8_t _ibuf[_BUFFER_SIZE];	/* input buffer */

  IPAddress _destIP;		/* remote IP address for outgoing packet during construction */
  uint16_t _destPort;		/* remote port for outgoing packet during construction */
  uint16_t _offset;		/* offset into output buffer */
  uint8_t _obuf[_BUFFER_SIZE];	/* output buffer */

public:
  // Constructor
 LinuxUDP() : _sock(-1) {}

  /* Start LinuxUDP socket, listening at local port PORT */
  // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use  
  uint8_t begin(uint16_t port) { 
    if ((_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) return 0;
    fcntl(_sock, F_SETFL, O_NONBLOCK);

    struct sockaddr_in myaddr;
    /* bind to an arbitrary return address */
    /* because this is the client side, we don't care about the address */
    /* since no application will initiate communication here - it will */
    /* just send responses */
    /* INADDR_ANY is the IP address and 0 is the socket */
    /* htonl converts a long integer (e.g. address) to a network representation */
    /* htons converts a short integer (e.g. port) to a network representation */
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* haha, its 0 so it could be either format */
    myaddr.sin_port = htons(port);
    if (bind(_sock, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) return 0;
    printf("begin(%d) returns true\n", port);
    return 1;
  }

  // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
  uint8_t beginMulticast(IPAddress, uint16_t) {
    return 0;
  }

  /* Release any resources being used by this LinuxUDP instance */
  // Finish with the UDP socket
  void stop() { if (_sock >= 0) ::close(_sock); _sock = -1; }

  // Sending UDP packets
  // Start building up a packet to send to the remote host specific in ip and port
  // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
  int beginPacket(IPAddress ip, uint16_t port) {
    printf("beginPacket((IPAddress)%s, %d)\n", ip.toString(), port);
    _offset = 0;
    _destIP = ip;
    _destPort = port;
    return 1;
  }

  // Start building up a packet to send to the remote host specific in host and port
  // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
  int beginPacket(const char *host, uint16_t port) {
    IPAddress addr;
    printf("beginPacket((char *)%s, %d)\n", host, port);
    return (addr.fromString(host)) ? beginPacket(addr, port) : 0;
  }

  // Finish off this packet and send it
  // Returns 1 if the packet was sent successfully, 0 if there was an error
  int endPacket() { 
    struct sockaddr_in servaddr;    /* server address */
    /* fill in the server's address and data */
    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr = _destIP.sockaddr();
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(_destPort);
    printf("sendto %s:%d\n", _destIP.toString(), _destPort);
    return sendto(_sock, _obuf, _offset, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
  }

  // Write a single byte into the packet
  size_t write(uint8_t byte) { 
    if (_offset+1 > _BUFFER_SIZE) return 0;
    _obuf[_offset++] = byte;
    return 1;
  }

  // Write size bytes from buffer into the packet
  size_t write(const uint8_t *buffer, size_t size) {
    if (_offset+size > _BUFFER_SIZE) return 0;
    memcpy(_obuf+_offset, buffer, size);
    _offset += size;
    return size;
  }
  
  // Start processing the next available incoming packet
  // Returns the size of the packet in bytes, or 0 if no packets are available
  int parsePacket() {
    // discard any remaining bytes in the last packet
    _ioffset = _ilimit = 0;
    
    struct sockaddr_in remaddr;	   /* remote address */
    socklen_t addrlen = sizeof(remaddr); /* length of addresses */
    _ilimit = recvfrom(_sock, _ibuf, _BUFFER_SIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
    if (_ilimit < 0) _ilimit = 0;
    return available();
  }
 
  /* return number of bytes available in the current packet,
     will return zero if parsePacket hasn't been called yet */
  // Number of bytes remaining in the current packet
  int available() { return _ilimit - _ioffset; }

  // Read a single byte from the current packet
  int read() {
    if (available() > 0) return _ibuf[_ioffset++];
    return -1;
  }

  // Read up to len bytes from the current packet and place them into buffer
  // Returns the number of bytes read, or 0 if none are available
  int read(unsigned char* buffer, size_t len) {
    int remaining = available();
    if (remaining > 0) {
      int got = (remaining <= len) ? remaining : len;
      memcpy(buffer, _ibuf+_ioffset, got);
      _ioffset += got;
      return got;
    }
    return -1;
  }

  // Read up to len characters from the current packet and place them into buffer
  // Returns the number of characters read, or 0 if none are available
  int read(char* buffer, size_t len) { return read((unsigned char*)buffer, len); };

  // Return the next byte from the current packet without moving on to the next byte
  int peek() { return available() > 0 ? _ibuf[_ioffset] : -1; }

  // Finish reading the current packet
  void flush() {}

  // Return the IP address of the host who sent the current incoming packet
  IPAddress remoteIP() { return _remoteIP; };

  // Return the port of the host who sent the current incoming packet
  uint16_t remotePort() { return _remotePort; };
};

#endif
