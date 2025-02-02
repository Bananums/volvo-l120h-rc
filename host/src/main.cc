//Created by Bananums: https://github.com/Bananums

#include <iostream>
#include <cerrno>
#include <cstring>

#include "serial_interface/serial_interface.h"

int main() {
    const char *uart_port_name = "/dev/ttyUSB1";

    int serial_port;
    if (!OpenSerialPort(uart_port_name, serial_port)) {
        std::cerr << "Failed to open serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        return 1;
    }

    if (!ConfigureSerialPort(serial_port, B115200)) {
        std::cerr << "Failed to configure serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        CloseSerialPort(serial_port);
    }

    const char *data = "Hello World!";
    const ssize_t bytes_written = WriteSerialData(serial_port, data);
    if (bytes_written < 0) {
        std::cerr << "Failed to write to serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "Successfully wrote: " << bytes_written << " bytes to: " << uart_port_name << std::endl;

    WaitForAllSerialTransmitted(serial_port);

    std::cout << "Closing serial port: " << uart_port_name << std::endl;
    CloseSerialPort(serial_port);

    return 0;
}
