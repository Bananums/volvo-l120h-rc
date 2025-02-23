//Created by Bananums: https://github.com/Bananums

#ifndef ENUMS_NANNERS_H
#define ENUMS_NANNERS_H

typedef enum {
    WAIT_FOR_SOF,
    READ_FRAME_ID,
    READ_LENGTH,
    READ_PAYLOAD,
    READ_CRC,
    VERIFY_EOF
} NannersState;


#endif //ENUMS_NANNERS_H
