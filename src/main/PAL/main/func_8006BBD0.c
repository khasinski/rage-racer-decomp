#include "common.h"
#include "psyq/cd.h"

extern CdRegisterMap *volatile D_80099314;
extern u8 *D_80099300;
extern u8 *D_80099304;
extern u8 *D_80099308;
extern u8 *D_8009930C;

int CD_initvol(void) {
    CdRegisterMap *temp_v1;
    u8 sp0[4];

    temp_v1 = D_80099314;

    if (temp_v1->status_mode_a == 0) {
        if (temp_v1->status_mode_b == 0) {
            temp_v1->cd_left_volume = 0x3FFF;
            temp_v1->cd_right_volume = 0x3FFF;
            temp_v1 = D_80099314;
        }
    }

    temp_v1->output_left_volume = 0x3FFF;
    temp_v1->output_right_volume = 0x3FFF;
    temp_v1->audio_control = 0xC001;

    sp0[2] = 0x80;
    sp0[0] = 0x80;
    sp0[3] = 0;
    sp0[1] = 0;

    *D_80099300 = 2;
    *D_80099308 = sp0[0];
    *D_8009930C = sp0[1];
    *D_80099300 = 3;
    *D_80099304 = sp0[2];
    *D_80099308 = sp0[3];
    *D_8009930C = 0x20;

    return 0;
}
