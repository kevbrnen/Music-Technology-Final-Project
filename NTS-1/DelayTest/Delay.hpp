#pragma once

#include "userdelfx.h"
#include <delayline.hpp>

#define MAX_DELAY_SAMPLES 48000
#define MAX_DELAY_MS 1000

void DELFX_INIT(uint32_t platform, uint32_t api);

void DELFX_PROCESS(float *xn, uint32_t frames);

void DELFX_PARAM(uint8_t index, int32_t value);
