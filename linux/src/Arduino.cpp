#include "Arduino.h"

unsigned long millis(void) {
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    if (ms > 999) {
        s++;
        ms = 0;
    }
    return s*1000 + ms;
}
unsigned long micros(void) {
    long            us; // Microseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    us = round(spec.tv_nsec / 1.0e3); // Convert nanoseconds to microsseconds
    if (us > 999999) {
        s++;
        us = 0;
    }
    return s*1000000 + us;
}

void Serial_::begin(unsigned long /* baud_count */) {}

void Serial_::begin(unsigned long /* baud_count */, byte /* config */) {}

void Serial_::end(void) {}

int Serial_::available(void) { return 1; }

int Serial_::peek(void) { int c = getc(stdin); ungetc(c, stdin); return c; }

int Serial_::read(void) { return getc(stdin); }

int Serial_::availableForWrite(void) { return 64*1024; }

void Serial_::flush(void) { fflush(stdout); }

size_t Serial_::write(uint8_t c) { return write(&c, 1); }

size_t Serial_::write(const uint8_t *buffer, size_t size) { return fwrite(buffer, 1, size, stdout); }

// This operator is a convenient way for a sketch to check whether the
// port has actually been configured and opened by the host (as opposed
// to just being connected to the host).  It can be used, for example, in 
// setup() before printing to ensure that an application on the host is
// actually ready to receive and display the data.
// We add a short delay before returning to fix a bug observed by Federico
// where the port is configured (lineState != 0) but not quite opened.
Serial_::operator bool() { return true; }

// unsigned long Serial_::baud() { return 38400; }

uint8_t Serial_::stopbits() { return 0; }

uint8_t Serial_::paritytype() { return 0; }

uint8_t Serial_::numbits() { return 0; }

bool Serial_::dtr() { return 0; }

bool Serial_::rts() { return 0; }

int32_t Serial_::readBreak() { return 0; }

Serial_ Serial;
