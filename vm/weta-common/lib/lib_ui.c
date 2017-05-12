#include <weta_platform.h>
#include <weta.h>
#include "lib_ui.h"
#include "../WvmCodes.h"
#include "../hw/hw_gpio.h"

static void lib_ui_simple_config(Weta* pWeta);
static void lib_ui_led_on(Weta* pWeta);
static void lib_ui_led_off(Weta* pWeta);

void WETAFUNCATTR
lib_ui_handler(Weta* pWeta)
{
    if (weta_store_read_byte(pWeta->store, pWeta->regs.pc, &pWeta->regs.opCode) == 0) {
        DEBUGMSG("Unable to read byte from code store.\n");
        weta_reset(pWeta);
        return;
    }
    DEBUGMSG("lib_ui_handler() - weta_store_read_byte(%d) = %d\r\n", pWeta->regs.pc, pWeta->regs.opCode);
    pWeta->regs.pc++;

    //sprintf(szMsg, "weta_store_read_byte(%d) = %d\r\n", loc, pWeta->regs.opCode);
    //weta_debug(pWeta, szMsg);
    switch (pWeta->regs.opCode) {
    case OP_UI_SIMPLE_CONFIG:
        lib_ui_simple_config(pWeta);
        break;

    case OP_UI_LED_ON:
        lib_ui_led_on(pWeta);
        break;

    case OP_UI_LED_OFF:
        lib_ui_led_off(pWeta);
        break;
    }
}

static void WETAFUNCATTR
lib_ui_simple_config(Weta* pWeta)
{
    uint8_t beeperPwm;
    uint8_t ledDigital;
    uint8_t id;

    weta_stack_popUint8(pWeta->stack, &beeperPwm);
    weta_stack_popUint8(pWeta->stack, &ledDigital);
    weta_stack_popUint8(pWeta->stack, &id);

    DEBUGMSG("lib_ui_simple_config(UserLED: %d, Beeper: %d)\r\n", ledDigital, beeperPwm);
    pWeta->hal->ledGpio = ledDigital;
    pWeta->hal->beeperPwm = beeperPwm;
}

static void WETAFUNCATTR
lib_ui_led_on(Weta* pWeta)
{
    if (pWeta->hal->ledGpio != 0xFF)
        hw_gpio_set(pWeta->hal, pWeta->hal->ledGpio, true);
}

static void WETAFUNCATTR
lib_ui_led_off(Weta* pWeta)
{
    if (pWeta->hal->ledGpio != 0xFF)
        hw_gpio_set(pWeta->hal, pWeta->hal->ledGpio, false);
}

