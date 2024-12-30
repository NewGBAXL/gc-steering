#ifndef GCSTEERING_H_INCLUDED
#define GCSTEERING_H_INCLUDED

#include <gctypes.h>

/**
 * Attempts to detect the presence of a GC Steering Wheel connected to any of the controller ports.
 * Returns the SI channel of the found steering wheel, or -1 if one could not be found.
 */
int GCSteering_Detect(void);

/**
 * Initializes the steering wheel, previously detected on the given SI channel. This must be called before
 * key press information can be read. Returns 1 on success, 0 on failure.
 */
int GCSteering_Init(int chan);

/**
 * Reads current key press information from the previously initialized steering wheel, located on the given
 * SI channel. Returns 1 if key press data has been returned, or 0 on failure.
 */
int GCSteering_ReadData(int chan, u8* wheelPosition, u16* buttons, u8* triggerL, u8* triggerR, u8* accel, u8* brake);

#endif
