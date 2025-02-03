//Created by Bananums: https://github.com/Bananums

#ifndef STRUCTS_H
#define STRUCTS_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

//Disable padding to ensure prdictable binary form
#pragma pack(push, 1)

typedef struct {
    uint8_t heartbeat;
    uint8_t function_request;
    float steering;
    float throttle;
    float tilt;
    float lift;
} CommandPayload;

typedef struct {
    uint8_t heartbeat;
    uint8_t gear;
    uint8_t event;
    float speed;
    float throttle;
    float brake;
    float lift;
} FeedbackPayload;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif //STRUCTS_H
