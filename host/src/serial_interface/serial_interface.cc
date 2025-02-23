//Created by Bananums: https://github.com/Bananums

#include "serial_interface.h"

#include <cstring>
#include <cassert>
#include <fcntl.h>

bool IsValidBaudRate(const speed_t baud_rate) {
    switch (baud_rate) {
        case B57600:
        case B115200:
        case B230400:
        case B460800:
        case B500000:
        case B576000:
        case B921600:
        case B1000000:
        case B1152000:
        case B1500000:
        case B2000000:
        case B2500000:
        case B3000000:
        case B3500000:
        case B4000000:
            return true;
        default:
            return false;
    }
};

bool OpenSerialPort(const char *port_name, int &file_descriptor) {
    assert(port_name != nullptr && "port_name must not be null!");
    file_descriptor = open(port_name, O_RDWR);

    if (file_descriptor < 0) {
        return false;
    }
    return true;
}

bool ConfigureSerialPort(const int file_descriptor, const speed_t baud_rate) {
    assert(file_descriptor >= 0 && "file_descriptor must be valid (file_descriptor >= 0)!");
    assert(IsValidBaudRate(baud_rate) && "Invalid baud_rate passed to ConfigureSerialPort!");

    termios tty{};
    if (tcgetattr(file_descriptor, &tty) != 0) {
        return false;
    }

    cfsetospeed(&tty, baud_rate);
    cfsetispeed(&tty, baud_rate);

    // Put the device into raw mode (this disables canonical mode, echo, and signals)
    cfmakeraw(&tty);

    tty.c_cflag &= ~PARENB; // Disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    if (tcsetattr(file_descriptor, TCSANOW, &tty) != 0) {
        return false;
    }

    return true;
}

void CloseSerialPort(const int file_descriptor) {
    close(file_descriptor);
}

void WaitForAllSerialTransmitted(const int file_descriptor) {
    tcdrain(file_descriptor);
}

ssize_t WriteSerialData(const int file_descriptor, const char *buffer) {
    assert(buffer != nullptr && "buffer must not be null!");
    assert(file_descriptor >= 0 && "file_descriptor must be valid (>= 0)!");

    const ssize_t bytes_written = write(file_descriptor, buffer, strlen(buffer));
    return bytes_written;
}

ssize_t WriteSerialCommand(int file_descriptor, const CommandPayload &command_payload) {
    assert(file_descriptor >= 0 && "file_descriptor must be valid (>= 0)!");

    const ssize_t bytes_written = write(file_descriptor, &command_payload, sizeof(command_payload));
    return bytes_written;
}

ssize_t WriteNannersCommand(int file_descriptor, const NannersFrame &frame) {
    assert(file_descriptor >= 0 && "file_descriptor must be valid (>= 0)!");

    const ssize_t bytes_written = write(file_descriptor, &frame, sizeof(frame));
    return bytes_written;
}

ssize_t ReadSerialData(const int file_descriptor, char *buffer, size_t const buffer_size) {
    assert(file_descriptor >= 0 && "file_descriptor must be valid (>= 0)!");
    const ssize_t bytes_read = read(file_descriptor, buffer, buffer_size);
    return bytes_read;
}
