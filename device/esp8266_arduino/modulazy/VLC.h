#ifndef MODULAZY_VLC_H
#define MODULAZY_VLC_H

#include <Arduino.h>

#define TIME_SLOT     250
#define BUFFER_LEN    64
#define MAX_DATA_PART 2
#define DELAY_ON_READ TIME_SLOT / 6

void VLCStartConfiguration();
void VLCStopConfiguration();
void VLCLoop();

#endif /* MODULAZY_VLC_H */
