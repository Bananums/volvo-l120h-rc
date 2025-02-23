//Created by Bananums: https://github.com/Bananums

#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include "enums.h"

enum : uint8_t {
    kMaxPayloadSize = 16, // Max payload size for all messages
    kStartOfFrame = 0xAA,
    kEndOfFrame = 0x55
};

// UART Frame Structure (Used for Transmission)
typedef struct __attribute__((packed)) {
    NannersState state;
    uint16_t frame_id;
    uint8_t length;
    uint8_t payload[kMaxPayloadSize];
    uint16_t crc;
    uint8_t index;
    bool valid;
} NannersFrame;

#endif //FRAME_H
