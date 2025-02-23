//Created by Bananums: https://github.com/Bananums

#include "nanners_protocol.h"
#include <string.h>
#include <stdio.h>

static NannersFrame frame = {
    .state = WAIT_FOR_SOF,
    .frame_id = 0,
    .length = 0,
    .payload = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    .crc = 0,
    .index = 0,
    .valid = false,
};

// Process each byte from UART
void NannersProcessBytes(uint8_t byte) {
    switch (frame.state) {
        case WAIT_FOR_SOF:
            if (byte == kStartOfFrame) { // Start of Frame
              printf("Received start of frame byte: %02X\n", byte);
              frame.state = READ_FRAME_ID;
              frame.index = 0;
            }
        break;

        case READ_FRAME_ID:
            //printf("Received frame id byte: %02X\n", byte);
            ((uint8_t*)&frame.frame_id)[frame.index++] = byte;
        if (frame.index == 2) {
            printf("Received frame id %d\n", frame.frame_id);
            frame.state = READ_LENGTH;
            frame.index = 0;
        }
        break;

        case READ_LENGTH:
            frame.length = byte;
            printf("Received frame length %d\n", frame.length);
        if (frame.length > kMaxPayloadSize) {
            frame.state = WAIT_FOR_SOF; // Invalid length, reset
        } else {
            frame.state = READ_PAYLOAD;
            frame.index = 0;
        }
        break;

        case READ_PAYLOAD:
            frame.payload[frame.index++] = byte;
            //printf("Received frame payload %02X\n", byte);
        if (frame.index >= frame.length) {
            frame.state = READ_CRC;
            frame.index = 0;
        }
        break;

        case READ_CRC:
            ((uint8_t*)&frame.crc)[frame.index++] = byte;
            //printf("Received CRC byte %02X\n", byte);
        if (frame.index == 2) {
              printf("Received CRC %u\n", frame.crc);
            frame.state = VERIFY_EOF;
            frame.index = 0; //TODO Check if necessray to reset here
        }
        break;

        case VERIFY_EOF:
            if (byte == kEndOfFrame) { // End of Frame
                printf("Received end of frame byte %02X\n", byte);
                //if (validate_crc(frame.payload, frame.length, frame.crc)) { //TODO add check
                    //printf("Valid frame\n");
                    frame.valid = true;
                //}
            }
        frame.state = WAIT_FOR_SOF;
        break;

        default:
            frame.state = WAIT_FOR_SOF;
        break;
    }
}

bool NannersGetFrame(NannersFrame *frame_out) {
  if(!frame.valid) {
    return false;
  }

  frame_out->frame_id = frame.frame_id;
  frame_out->length = frame.length;
  memcpy(frame_out->payload, frame.payload, frame.length);

  frame.valid = false;
  return true;
}