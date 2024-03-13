#include "Delay.hpp"

float DelTime;
static dsp::DelayLine DelayLine;
size_t DelIdx;
size_t delayBufferSize;
static __sdram float s_delay_ram[MAX_DELAY_SAMPLES+1];

void DELFX_INIT(uint32_t platform, uint32_t api)
{
    DelayLine.setMemory(s_delay_ram, delayBufferSize);
    DelTime = 0.20;
    DelIdx = 0;
    
}

void DELFX_PROCESS(float *xn, uint32_t frames)
{
    q31_t * __restrict x = (q31_t*)xn;

    const q31_t *x_e = x + 2 * frames;

    for(; x != x_e ;)
    {
        auto delayed = DelayLine.readFrac(DelIdx);

        DelayLine.write(*x);

        *(x++) = (0.6 * delayed) + (0.4 * *x);

        DelIdx = (DelIdx + 1) % static_cast<int>(DelTime * 48000);
    }

}

void DELFX_PARAM(uint8_t index, int32_t value)
{
  switch (index) {
  case k_user_delfx_param_time:
    DelTime = (value * MAX_DELAY_SAMPLES)/128;
    break;
  case k_user_delfx_param_depth:
    break;
  default:
    break;
  }
}
