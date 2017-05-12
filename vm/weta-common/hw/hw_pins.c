#include "hw_pins.h"
#include <stdbool.h>

#ifdef SUPPORT_JSON

static void WETAFUNCATTR
add_cap(WetaPinCaps pin, eWetaPinCaps caps, const char* attr, char * buf, bool* comma)
{
    if (pin & caps)
    {
        if (*comma)
            weta_strcat(buf, ",");
        weta_strcat(buf, attr);
        *comma = true;
    }

}

/**
 * @brief Convert the pin capabilities to JSON
 *
 * This requires a buffer of about 1500 bytes, so I am starting to think that
 * the information should simply be made available as a JSON file for each
 * platform.
 * @param buf
 * @param buflen
 * @return
 */
bool WETAFUNCATTR
weta_pins_to_json(char * buf, size_t buflen)
{
    // TODO: Properly handle buffer length
    buf[0] = '\0';
    weta_strcat(buf, "{ \"pins\": [");
    int i;
    bool comma = false;
    for (i = 0; i < weta_pins.n_pins; i++)
    {
        WetaPinCaps caps = weta_pins.pins[i];
        if (caps & PINCAP_AVOID)
            continue;
        if (comma)
            weta_strcat(buf, ",");
        comma = false;
        weta_sprintf(buf + weta_strlen(buf), "{\"pin\":%d,", i);

        add_cap(caps, PINCAP_DIGITAL_IN, "\"din\":1", buf, &comma);
        add_cap(caps, PINCAP_DIGITAL_OUT, "\"dout\":1", buf, &comma);
        add_cap(caps, PINCAP_PWM, "\"pwm\":1", buf, &comma);
        add_cap(caps, PINCAP_ADC, "\"adc\":1", buf, &comma);
        add_cap(caps, PINCAP_DAC, "\"dac\":1", buf, &comma);
        add_cap(caps, PINCAP_COUNTER, "\"counter\":1", buf, &comma);
        add_cap(caps, PINCAP_TOUCH, "\"touch\":1", buf, &comma);
        add_cap(caps, PINCAP_I2C_SCL, "\"i2c_scl\":1", buf, &comma);
        add_cap(caps, PINCAP_I2C_SDA, "\"i2c_sda\":1", buf, &comma);
        add_cap(caps, PINCAP_SPI_MISO, "\"spi_miso\":1", buf, &comma);
        add_cap(caps, PINCAP_SPI_MOSI, "\"spi_mosi\":1", buf, &comma);
        add_cap(caps, PINCAP_SPI_SCLK, "\"spi_sclk\":1", buf, &comma);
        add_cap(caps, PINCAP_SPI_SS, "\"spi_ss\":1", buf, &comma);
        add_cap(caps, PINCAP_UART_RX, "\"uart_rx\":1", buf, &comma);
        add_cap(caps, PINCAP_UART_TX, "\"uart_tx\":1", buf, &comma);
        add_cap(caps, PINCAP_UART_RTS, "\"uart_rts\":1", buf, &comma);
        add_cap(caps, PINCAP_UART_CTS, "\"uart_cts\":1", buf, &comma);
        weta_strcat(buf, "}");
        comma = true;
    }
    weta_strcat(buf, "]"); //,\"gpio_options\":[\"in\",\"out\",\"pullup\",\"pulldown\",\"opendrain\",\"invert\",\"debounce\"]");
    if (weta_pins.n_adc)
    {
        weta_strcat(buf, ", \"adc_order\": [");
        for (i = 0; i < weta_pins.n_adc; i++)
        {
            weta_sprintf( buf + weta_strlen(buf), "%d", weta_pins.adc_order[i]);
            if (i < weta_pins.n_adc - 1)
                weta_strcat(buf, ",");
        }
        weta_strcat(buf, "]");
    }
    if (weta_pins.n_dac)
    {
        weta_strcat(buf, ", \"dac_order\": [");
        for (i = 0; i < weta_pins.n_dac; i++)
        {
            weta_sprintf( buf + weta_strlen(buf), "%d", weta_pins.dac_order[i]);
            if (i < weta_pins.n_dac - 1)
                weta_strcat(buf, ",");
        }
        weta_strcat(buf, "]");
    }
    if (weta_pins.n_touch)
    {
        weta_strcat(buf, ", \"touch_order\": [");
        for (i = 0; i < weta_pins.n_touch; i++)
        {
            weta_sprintf( buf + weta_strlen(buf), "%d", weta_pins.touch_order[i]);
            if (i < weta_pins.n_touch - 1)
                weta_strcat(buf, ",");
        }
        weta_strcat(buf, "]");
    }
    if (weta_pins.n_uart_groups)
    {
        weta_strcat(buf, ", \"uart_groups\": [");
        for (i = 0; i < weta_pins.n_uart_groups; i++)
        {
            weta_sprintf(
                buf + weta_strlen(buf),
                "{\"tx\":%d, \"rx\":%d, \"cts\":%d, \"rts\":%d}",
                weta_pins.uart_groups[i].tx,
                weta_pins.uart_groups[i].rx,
                weta_pins.uart_groups[i].cts,
                weta_pins.uart_groups[i].rts
            );
            if (i < weta_pins.n_uart_groups - 1)
                weta_strcat(buf, ",");
        }
        weta_strcat(buf, "]");
    }
    if (weta_pins.n_spi_groups)
    {
        weta_strcat(buf, ", \"spi_groups\": [");
        for (i = 0; i < weta_pins.n_spi_groups; i++)
        {
            weta_sprintf(
                buf + weta_strlen(buf),
                "{\"miso\":%d, \"mosi\":%d, \"sclk\":%d, \"ss\":%d}",
                weta_pins.spi_groups[i].miso,
                weta_pins.spi_groups[i].mosi,
                weta_pins.spi_groups[i].sclk,
                weta_pins.spi_groups[i].ss
            );
            if (i < weta_pins.n_spi_groups - 1)
                weta_strcat(buf, ",");
        }
        weta_strcat(buf, "]");
    }
    if (weta_pins.n_i2c_groups)
    {
        weta_strcat(buf, ", \"i2c_groups\": [");
        for (i = 0; i < weta_pins.n_i2c_groups; i++)
        {
            weta_sprintf(
                buf + weta_strlen(buf),
                "{\"scl\":%d, \"sda\":%d}",
                weta_pins.i2c_groups[i].scl,
                weta_pins.i2c_groups[i].sda
            );
            if (i < weta_pins.n_i2c_groups - 1)
                weta_strcat(buf, ",");
        }
        weta_strcat(buf, "]");
    }
    weta_strcat(buf, "}");

    return true; // For now
}
#endif  // SUPPORT_JSON