#include <iostream>
#include <fcntl.h>      // For O_RDWR, O_NOCTTY, etc.
#include <unistd.h>     // For close(), write(), tcdrain()
#include <termios.h>    // For termios, tcgetattr(), tcsetattr(), cfsetispeed(), cfsetospeed(), cfmakeraw()
#include <cstring>
#include <cerrno>


bool OpenSerialPort(const char *port_name, int &file_descriptor) {
    file_descriptor = open(port_name, O_RDWR);

    if (file_descriptor < 0) {
        return false;
    }
    return true;
}

bool ConfigureSerialPort(const int file_descriptor, const speed_t baud_rate) {
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
    const ssize_t bytes_written = write(file_descriptor, buffer, strlen(buffer));
    return bytes_written;
}


int main() {
    const char *uart_port_name = "/dev/ttyUSB1";

    int serial_port;
    if (!OpenSerialPort(uart_port_name, serial_port)) {
        std::cerr << "Failed to open serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        return 1;
    }

    if (!ConfigureSerialPort(serial_port, 115200)) {
        std::cerr << "Failed to configure serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        CloseSerialPort(serial_port);
    }


    const char *data = "Helllo World!";
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
