//Created by Bananums: https://github.com/Bananums

#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <unistd.h>
#include <termios.h>

// Checks if the given baud_rate matches one of the known baud rate constants.
bool IsValidBaudRate(speed_t baud_rate);

// Opens a serial port for read/write.
// Returns true on success, false otherwise.
// On success, the file descriptor is stored in `out_fd`.
bool OpenSerialPort(const char *port_name, int &file_descriptor);

// Configures the serial port (baud rate, raw mode, etc.).
// Returns true on success, false otherwise.
bool ConfigureSerialPort(int file_descriptor, speed_t baud_rate);

// Writes data to the serial port.
// Returns the number of bytes written on success, or -1 on error.
void CloseSerialPort(int file_descriptor);

// Waits until all written data has been transmitted.
void WaitForAllSerialTransmitted(int file_descriptor);

// Closes the serial port.
ssize_t WriteSerialData(int file_descriptor, const char *buffer);

#endif //SERIAL_INTERFACE_H
