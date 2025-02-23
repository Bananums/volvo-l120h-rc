//Created by Bananums: https://github.com/Bananums

#include <iostream>
#include <cerrno>
#include <cstring>
#include <memory>
#include <csignal>
#include <atomic>
#include <chrono>
#include <thread>

#include "serial_interface/serial_interface.h"
#include "structs.h"

std::atomic<bool> stop_flag(false);

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    stop_flag = true;
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    const char *uart_port_name = "/dev/ttyUSB1";
    //const char *uart_port_name = "/dev/pts/2";

    CommandPayload test_command{
        .heartbeat = 97,
        .function_request = 43,
        .steering = -0.2345,
        .throttle = 1.23,
        .tilt = 12.452,
        .lift = -1234.235,
    };

    std::cout << "Command size: " << sizeof(CommandPayload) << std::endl;

    int serial_port;
    if (!OpenSerialPort(uart_port_name, serial_port)) {
        std::cerr << "Failed to open serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        return 1;
    }

    if (!ConfigureSerialPort(serial_port, B115200)) {
        std::cerr << "Failed to configure serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        CloseSerialPort(serial_port);
    }

    //const char *data = "Hello World!\n";

    //ssize_t bytes_written = WriteSerialData(serial_port, data);
    //if (bytes_written < 0) {
    //    std::cerr << "Failed to write to serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
    //    return 1;
    //}

    HeartbeatPayload heartbeat_payload;
    heartbeat_payload.heartbeat = 7;
    //drive_payload.steering = 12.45;
    //drive_payload.throttle = 1.23;

    NannersFrame send_frame;
    send_frame.start_frame = kStartOfFrame;
    send_frame.frame_id = 1002;
    send_frame.crc = 2;

    if (sizeof(DrivePayload) > kMaxPayloadSize) {
        //TODO handle error
    }

    memset(send_frame.payload, 0, kMaxPayloadSize);
    memcpy(send_frame.payload, &heartbeat_payload, sizeof(HeartbeatPayload));
    std::cout << "Length: " << +kMaxPayloadSize << std::endl;
    send_frame.length = kMaxPayloadSize;
    send_frame.end_frame = kEndOfFrame;

    ssize_t bytes_written = WriteNannersCommand(serial_port, send_frame);
    if (bytes_written < 0) {
        std::cerr << "Failed to write to serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        return 1;
    }


    DrivePayload drive_payload;
    drive_payload.heartbeat = 9;
    drive_payload.steering = 12.45;
    drive_payload.throttle = 1.23;
    send_frame.frame_id = 1004;
    send_frame.crc = 123;

    memset(send_frame.payload, 0, kMaxPayloadSize);
    memcpy(send_frame.payload, &drive_payload, sizeof(DrivePayload));

    bytes_written = WriteNannersCommand(serial_port, send_frame);
    if (bytes_written < 0) {
        std::cerr << "Failed to write to serial port " << uart_port_name << ": " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "Successfully wrote: " << bytes_written << " bytes to: " << uart_port_name << std::endl;

    ssize_t bytes_received = 0;
    char byte = 0;
    while (!stop_flag) {
        bytes_received = ReadSerialData(serial_port, &byte, sizeof(byte));
        if (bytes_received > 0) {
            std::cout << "Received: " << +static_cast<uint8_t>(byte) << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    WaitForAllSerialTransmitted(serial_port);

    std::cout << "Closing serial port: " << uart_port_name << std::endl;
    CloseSerialPort(serial_port);

    return 0;
}
