#include <gccore.h>
#include <ogc/pad.h>
#include <stdio.h>

#if defined(HW_DOL)
#define SI_REG_BASE 0xCC006400
#elif defined(HW_RVL)
#define SI_REG_BASE 0xCD006400
#else
#error Hardware model unknown? Missing a preprocessor definition somewhere...
#endif

#define SIREG(n)               ((vu32*)(SI_REG_BASE + (n)))

#define SICOUTBUF(n)           (SIREG(0x00 + (n)*12))               /* SI Channel n Output Buffer (Joy-channel n Command) (4 bytes) */
#define SICINBUFH(i)           (SIREG(0x04 + (i)*12))               /* SI Channel n Input Buffer High (Joy-channel n Buttons 1) (4 bytes) */
#define SICINBUFL(i)           (SIREG(0x08 + (i)*12))               /* SI Channel n Input Buffer Low (Joy-channel n Buttons 2) (4 bytes) */
#define SIPOLL                 (SIREG(0x30))                        /* SI Poll Register (4 bytes) */
#define SICOMCSR               (SIREG(0x34))                        /* SI Communication Control Status Register (command) (4 bytes) */
#define SISR                   (SIREG(0x38))                        /* SI Status Register (4 bytes) */
#define SIIOBUF                (SIREG(0x80))                        /* SI I/O buffer (access by word) (128 bytes) */

#define PAD_ENABLEDMASK(chan)  (0x80000000 >> chan)

static void SI_AwaitPendingCommands(void) {
    while (*SICOMCSR & 0x1);
}

int GCSteering_Detect(void) {
    SI_AwaitPendingCommands();

    u32 buf[2];
    for (int i = 0; i < 4; ++i) {
        SI_GetResponse(i, buf);
        SI_SetCommand(i, 0x00400300);
        SI_EnablePolling(PAD_ENABLEDMASK(i));
    }
    SI_AwaitPendingCommands();

    int steeringChan = -1;

    for (int i = 0; i < 4; ++i) {
        u32 type = SI_DecodeType(SI_GetType(i));
        if (type == SI_GC_STEERING) {
            steeringChan = i;
        }
    }

    return steeringChan;
}

int GCSteering_Init(int chan) {
    if (chan == -1)
        return 0;

    u32 buf[2];

    SI_GetResponse(chan, buf);
	//SI_SetCommand(chan, 0x00540000); //was causing a crash on real hardware
    SI_EnablePolling(PAD_ENABLEDMASK(chan));
    SI_TransferCommands();
    SI_AwaitPendingCommands();

    return 1;
}

int GCSteering_ReadData(int chan, u8* wheelPosition, u16* buttons, u8* triggerL, u8* triggerR, u8* accel, u8* brake) {
    if (chan == -1) {
        return 0;
    }
    if (!wheelPosition || !buttons || !triggerL || !triggerR || !accel || !brake) {
        return 0;
    }

    u32 buffer[2];
    if (SI_GetResponse(chan, buffer)) {
		*buttons = (buffer[0] >> 16) & 0xffff;
		//*pedalConnect = (buffer[0] >> 16) & 0x80; //pedal connected is the 8th bit of the buttons
        *wheelPosition = (buffer[0] >> 8) & 0xff;
		//*wheelPositionY = buffer[0] & 0xff; //not sure if real hardware can do this

		*accel = (buffer[1] >> 24) & 0xff;
		*brake = (buffer[1] >> 16) & 0xff;
		*triggerL = (buffer[1] >> 8) & 0xff;
		*triggerR = buffer[1] & 0xff;
        
        return 1;
    }
    else {
		*wheelPosition = 0;
		*buttons = 0;
		*triggerL = 0;
		*triggerR = 0;
		*accel = 0;
		*brake = 0;
    }
    return 0;
}

