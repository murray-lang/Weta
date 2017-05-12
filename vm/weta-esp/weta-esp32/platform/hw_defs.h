#ifndef __HW_DEFS_H__
#define __HW_DEFS_H__

typedef struct
{
    WetaAdcChannel  channel;
    WetaAdcGain     gain;
} AdcChannel;

typedef struct
{
    AdcChannel      channels[MAX_ADC];
    WetaAdcDepth    depth;
} Adc;

typedef struct
{
    uint8_t  width;
    uint16_t frequency;
} PwmTimer;
typedef struct
{
    WetaPwmChannel	channel;
    uint8_t         timer;
    WetaPin		    pin;
} PwmChannel;

typedef struct
{
    PwmTimer   timers[MAX_PWM_TIMERS];
    PwmChannel channels[MAX_PWM];
}PwmChannels;




#endif  //__HW_DEFS_H__