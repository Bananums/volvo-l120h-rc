//Created by Bananums: https://github.com/Bananums

#ifndef NANNERS_PROTOCOL_H
#define NANNERS_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include "frame.h"

void NannersProcessBytes(uint8_t byte);
bool NannersGetFrame(NannersFrame *frame_out);

#endif //NANNERS_PROTOCOL_H
