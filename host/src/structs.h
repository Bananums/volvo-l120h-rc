//Created by Bananums: https://github.com/Bananums

#ifndef STRUCTS_H
#define STRUCTS_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

enum : uint8_t {
    kMaxPayloadSize = 16, // Max payload size for all messages
    kStartOfFrame = 0xAA,
    kEndOfFrame = 0x55
};

typedef struct __attribute__((packed)) {
    const uint8_t start_frame = kStartOfFrame;
    uint8_t heartbeat;
    uint8_t function_request;
    float steering;
    float throttle;
    float tilt;
    float lift;
    const uint8_t end_frame = kEndOfFrame;
} CommandPayload;

typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    float steering;
    float throttle;
} DrivePayload;

typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
} HeartbeatPayload;

typedef struct __attribute__((packed)) {
    uint8_t start_frame;
    uint16_t frame_id;
    uint8_t length;
    uint8_t payload[kMaxPayloadSize];
    uint16_t crc;
    uint8_t end_frame;
} NannersFrame;

typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    float tilt;
    float lift;
} ToolPayload;

typedef struct __attribute__((packed)) {
    uint8_t function_request;
} FunctionPayload;

typedef struct __attribute__((packed)) {
    float steering;
    float throttle;
    float tilt;
    float lift;
} FeedbackPayload;


#ifdef __cplusplus
}
#endif

#endif //STRUCTS_H
