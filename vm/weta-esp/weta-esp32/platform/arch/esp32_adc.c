#include <hw.h>
#include <driver/adc.h>

    // Change width to the nearest available value
static uint8_t      snap_width(uint8_t width);
    // Convert width to an enum for the API
static adc_bits_width_t width_to_enum(uint8_t width);

    // Change gain to the nearest available value
static int8_t      snap_gain(int8_t gain);
    // Convert gain to an attenuation for the API
static adc_atten_t gain_to_atten(int8_t gain);


void WETAFUNCATTR
hw_adc_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;
    if (hw->adc.depth) {
        adc1_config_width(width_to_enum(hw->adc.depth));
        int i = 0;
        for (i = 0; i < MAX_ADC; i++) {
            if (hw->adc.channels[i].channel != 0xFF)
                adc1_config_channel_atten(
                    (adc1_channel_t) hw->adc.channels[i].channel,
                    gain_to_atten(hw->adc.channels[i].gain)
                );
        }
    }
}

bool WETAFUNCATTR
hw_adc_config(struct _Hardware* hw, uint8_t depth)
{
    hw->adc.depth = snap_width(depth);

    return adc1_config_width(width_to_enum(depth)) == ESP_OK;
}

bool WETAFUNCATTR
hw_adc_channel_config(struct _Hardware* hw, uint8_t channel, int8_t gain)
{
    if (channel >= MAX_ADC)
        return false;
        // This might seem superflous, but it marks the channel as in use
    hw->adc.channels[channel].channel = channel;
        // Adjust gain to available values
    hw->adc.channels[channel].gain = snap_gain(gain);

    return adc1_config_channel_atten(
        (adc1_channel_t)channel,
        (adc_atten_t)gain_to_atten(gain)
    ) == ESP_OK;
}

bool WETAFUNCATTR
hw_adc_get(struct _Hardware* hw, uint8_t i, int16_t* value)
{
    Adc* adc = &hw->adc;
    if (i < MAX_ADC)
        *value = (int16_t) adc1_get_voltage((adc1_channel_t)adc->channels[i].channel);
    else
        *value = 0;
    return true;
}

static uint8_t WETAFUNCATTR
snap_width(uint8_t width)
{
    if (width < 10)
        return 9;
    else if (width < 11)
        return 10;
    else if (width < 12)
        return 11;
    else
        return 12;
}

static adc_bits_width_t WETAFUNCATTR
width_to_enum(uint8_t width)
{
    if (width < 10)
        return ADC_WIDTH_9Bit;
    else if (width < 11)
        return ADC_WIDTH_10Bit;
    else if (width < 12)
        return ADC_WIDTH_11Bit;
    else
        return ADC_WIDTH_12Bit;
}

static int8_t WETAFUNCATTR
snap_gain(int8_t gain)
{
    if (gain >= 0)
        return 0;
    else if (gain > -6)
        return -3;  // Don't support fractions at this stage
    else if (gain > -11)
        return -6;
    else
        return -11;
}

static adc_atten_t WETAFUNCATTR
 gain_to_atten(int8_t gain)
{
    if (gain >= 0)
        return ADC_ATTEN_0db;
    else if (gain > -6)
        return ADC_ATTEN_2_5db;
    else if (gain > -11)
        return ADC_ATTEN_6db;
    else
        return ADC_ATTEN_11db;
}

