#include <hw.h>
#include <driver/ledc.h>

static bool config_pwm_channel(PwmChannel* ch);
static bool config_pwm_timer(uint8_t i, PwmTimer* t);

void WETAFUNCATTR
hw_pwm_init(struct _Hardware* hw, uint16_t flags)
{
    uint8_t i;
    for (i = 0; i < MAX_PWM_TIMERS; i++) {
        config_pwm_timer(i, &hw->pwms.timers[i]);
    }
    for (i = 0; i < MAX_PWM; i++) {
        config_pwm_channel(&hw->pwms.channels[i]);
    }
}

static bool WETAFUNCATTR
config_pwm_channel(PwmChannel* ch)
{
    if (ch->pin == 0xFF || ch->channel == 0xFF || ch->timer == 0xFF)
        return false;
    ledc_channel_config_t channel_config;
    channel_config.speed_mode   = LEDC_HIGH_SPEED_MODE; // Only high speed mode available at the moment
    channel_config.intr_type    = LEDC_INTR_DISABLE;    // No fade interrupt required
    channel_config.timer_sel    = ch->timer;         // Select the timer source of channel (0 - 3)
    channel_config.duty         = 0;                    // Leave off to start with

    channel_config.gpio_num     = ch->pin;
    channel_config.channel      = (ledc_channel_t)ch->channel;  // LEDC channel(0 - 7)
    return ledc_channel_config(&channel_config) == ESP_OK;
}

static bool WETAFUNCATTR
config_pwm_timer(uint8_t i, PwmTimer* t)
{
    if (t->width == 0 || t->frequency == 0 )
        return false;
    if (t->width < 10)
        t->width = 10;
    if (t->width > 15)
        t->width = 15;
    ledc_timer_config_t config;
    config.speed_mode = LEDC_HIGH_SPEED_MODE;   // Only high speed mode available at the moment
    config.bit_num    = (ledc_timer_bit_t)t->width;
    config.timer_num  = (ledc_timer_t)i;           // Just use one timer at this stage
    config.freq_hz    = (uint32_t)t->frequency;

    return ledc_timer_config(&config) == ESP_OK;
}

bool WETAFUNCATTR
hw_pwm_config_timer(struct _Hardware* hw, uint8_t timer, uint16_t frequency, uint8_t width)
{
    if (timer >= MAX_PWM_TIMERS || width < 10 || width > 15)
        return false; // TODO: error handling

    hw->pwms.timers[timer].frequency = frequency;
    hw->pwms.timers[timer].width = width;

    return config_pwm_timer(timer, &hw->pwms.timers[timer]);
}

bool WETAFUNCATTR
hw_pwm_config_channel(struct _Hardware* hw, WetaPwmChannel index, uint8_t gpio_num, uint8_t timer)
{
    if (index >= LEDC_CHANNEL_MAX)
        return false; // TODO: error handling

    hw->pwms.channels[index].pin     = gpio_num;
    hw->pwms.channels[index].channel = index;
    hw->pwms.channels[index].timer   = timer;

    return config_pwm_channel(&hw->pwms.channels[index]);
}

void WETAFUNCATTR
hw_pwm_set_duty(WetaPwmChannel channel, WetaPwmDuty duty)
{

    ledc_set_duty(
            LEDC_HIGH_SPEED_MODE,
            (ledc_channel_t) channel,
            (uint32_t) (duty * 1023) / 255
    );
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}
