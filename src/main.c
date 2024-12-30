#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <debug.h>
#include <string.h>

#include "gcsteering.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int main(int argc, char **argv) {

    VIDEO_Init();

    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

    printf("\n\nHello ...\n");

    //DEBUG_Init(GDBSTUB_DEVICE_USB,1);
    //printf("Waiting for debugger ...\n");
    //_break();
    //printf("debugger connected ...\n");

    PAD_Init();

    int steeringChan = GCSteering_Detect();
    printf("GC Steering Wheel located at chan %d\n", steeringChan);

    int steeringEnabled = GCSteering_Init(steeringChan);
    if (steeringEnabled) {
        printf("GC Steering Wheel initialized on chan %d\n", steeringChan);

    }

	u8 wheelPosition = 0;
	u16 buttons = 0;
	u8 triggerL = 0;
	u8 triggerR = 0;
	u8 accel = 0;
	u8 brake = 0;

    printf("Start of main loop.\nPress Start on the controller plugged into slot 1 to exit ...\n");
    while(1) {
        if (steeringEnabled) {
			if (GCSteering_ReadData(steeringChan, &wheelPosition, &buttons, &triggerL, &triggerR, &accel, &brake)) {
				printf("Steering Wheel Position: %d\n", wheelPosition);
				printf("Buttons: 0x%04X\n", buttons);
				printf("Trigger L: %d\n", triggerL);
				printf("Trigger R: %d\n", triggerR);
				printf("Pedal Connected: %d\n", (buttons >> 7) & 0x01);
				printf("Accel: %d\n", accel);
				printf("Brake: %d\n", brake);
            }
        }

        PAD_ScanPads();
        u32 pressed = PAD_ButtonsDown(0);
        if (pressed) {
            printf("PAD 0 buttons pressed: 0x%08x\n", pressed);
        }
        if ( pressed & PAD_BUTTON_START ) {
            printf("exiting ...\n");
            exit(0);
        }

        VIDEO_WaitVSync();
    }

    return 0;
}
