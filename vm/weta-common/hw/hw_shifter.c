#include "hw_shifter.h"
#include "hw.h"

void WETAFUNCATTR
hw_shift_init(struct _Hardware* hw, uint16_t flags)
{

}

bool WETAFUNCATTR
hw_shift_config(
    struct _Hardware* hw,
    uint8_t shifterid,
    uint8_t width,
    uint8_t data,
    uint8_t clock,
    uint8_t strobe
)
{
    if (shifterid >= MAX_SHIFTERS)
        return false;

    Shifter* shifter = &hw->shifters[shifterid];
    bool error = false;

    shifter->width = width;

    if (data < MAX_GPIO || data == 0xFF)
        shifter->data = data;
    else
        error = true;

    if (clock < MAX_GPIO || clock == 0xFF)
        shifter->clock = clock;
    else
        error = true;

    if (strobe < MAX_GPIO || strobe == 0xFF)
        shifter->strobe = strobe;
    else
        error = true;

    return !error;
}

bool WETAFUNCATTR
hw_shift_strobe(struct _Hardware* hw, uint8_t shifterid)
{
    if (shifterid >= MAX_SHIFTERS)
        return false;
    uint8_t pin = hw->shifters[shifterid].strobe;
    if (pin == 0xFF)
        return false;

    return hw_gpio_pulse(hw, pin);
}

uint8_t WETAFUNCATTR
hw_shift_out(struct _Hardware* hw, uint8_t shifterid, uint8_t count, bool strobe)
{
    //DEBUGMSG("hw_shift_out(%d, %d)\r\n", shifterid, count);
    if (shifterid >= MAX_SHIFTERS)
        return 0;
    Shifter* shifter = &hw->shifters[shifterid];

    if (   shifter->data == 0xFF
        || shifter->clock == 0xFF
        || shifter->strobe == 0xFF) {
        //DEBUGMSG("Shifter pins not configured\r\n");
        return 0;
    }

        // Prevent shifting past the width. This means that the caller need not
        // know how much needs to be shifted - just 0xFF, say, will cause all
        // the data, and no more, to be shifted out. It's bad to keep shifting
        // as the position of all the bits in the shift register is significant.
    uint8_t remaining = shifter->width - shifter->shifted;
    if (remaining == 0)
        return 0;
    uint8_t amt = count < remaining ? count : remaining;

    //DEBUGMSG("To GPIO %d:", shifter->data);
    uint8_t i;
    for (i = 0; i < amt; i++)
    {
        bool next = (shifter->buffer[i / 8] & (1<<(i % 8))) != 0;
        //DEBUGMSG("%d", next ? 1 : 0);
        hw_gpio_set(hw, shifter->data, next);
        hw_gpio_pulse(hw, shifter->clock);
    }
    //DEBUGMSG("\r\n");
    if (strobe)
        hw_gpio_pulse(hw, shifter->strobe);

    shifter->shifted += amt;
    return amt;
}

uint8_t WETAFUNCATTR
hw_shift_in(struct _Hardware* hw, uint8_t shifterid, uint8_t count, bool strobe)
{
    if (shifterid >= MAX_SHIFTERS)
        return 0;
    Shifter* shifter = &hw->shifters[shifterid];

    if (shifter->data == 0xFF || shifter->clock == 0xFF || shifter->strobe == 0xFF)
        return 0;

        // Prevent shifting past the width. This means that the caller need not
        // know how much needs to be shifted - just 0xFF, say, will cause all
        // the data, and no more, to be shifted in. It's bad to keep shifting
        // as the position of all the bits in the shift register is significant.
    uint8_t remaining = shifter->width - shifter->shifted;
    if (remaining == 0)
        return 0;
    uint8_t amt = count < remaining ? count : remaining;

    if (strobe)
        hw_gpio_pulse(hw, shifter->strobe);

    uint8_t i;
    for (i = 0; i < amt; i++)
    {
        hw_gpio_pulse(hw, shifter->clock);
        bool next;
        hw_gpio_get(hw, shifter->data, &next);
        uint8_t bit = 1 << (i % 8);
        shifter->buffer[i / 8] &= ~bit;
        if (next)
            shifter->buffer[i/8] |= bit;
    }
    shifter->shifted += amt;
    return true;
}

bool WETAFUNCATTR
hw_shift_set_buffer(
    struct _Hardware* hw,
    uint8_t shifterid,
    uint8_t bits,
    uint8_t offset,
    uint8_t count
)
{
    //DEBUGMSG("hw_shift_set_buffer(%d, %0X, %d, %d)\r\n", shifterid, bits, offset, count);
    if (shifterid >= MAX_SHIFTERS)
        return false;

    Shifter* shifter = &hw->shifters[shifterid];
    shifter->shifted = 0;    // Reset if the buffer is touched
    uint8_t i;
    for (i = 0; i < count; i++)
    {
        uint8_t byteOff = (offset + i) / 8;
        uint8_t bitOff = (offset + i) % 8;

        uint8_t targetmask = 1 << bitOff;
        shifter->buffer[byteOff] &= ~targetmask; // Clear the target bit
            //If the next source bit is set then set the target bit
        if (bits & (1 << i))
            shifter->buffer[byteOff] |= targetmask;
    }
    return true;
}

bool WETAFUNCATTR
hw_shift_get_buffer(
    struct _Hardware* hw,
    uint8_t shifterid,
    uint8_t* bits,
    uint8_t offset,
    uint8_t count
)
{
    if (shifterid >= MAX_SHIFTERS)
        return false;

    Shifter* shifter = &hw->shifters[shifterid];
    shifter->shifted = 0;    // Reset if the buffer is touched
    uint8_t i;
    for (i = 0; i < count; i++)
    {
        uint8_t byteOff = (offset + i) / 8;
        uint8_t bitOff = (offset + i) % 8;

        uint8_t sourcemask = 1 << bitOff;
        *bits &= ~(1 << i);     // Clear the output bit
            //If the next buffer bit is set then set the output bit
        if (shifter->buffer[byteOff] & sourcemask)
            *bits |= 1 << i;
    }
    return true;
}

