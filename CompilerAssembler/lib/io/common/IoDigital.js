var gpioDefines = {
    GPIO_OPT_NONE:       0x00,
    GPIO_OPT_INPUT:      0x01,
    GPIO_OPT_OUTPUT:     0x02,
    GPIO_OPT_PULLUP:     0x04,
    GPIO_OPT_PULLDOWN:   0x08,
    GPIO_OPT_OPEN_DRAIN: 0x10,
    GPIO_OPT_INVERT:     0x20,
    GPIO_OPT_DEBOUNCE:   0x40
};

function gpioSetDefines(output)
{
    // Databits, parity and stop bits are packed into a byte as fields
    output.defines["GPIO_OPT_NONE"]       = "0x" + gpioDefines.GPIO_OPT_NONE.toString(16);
    output.defines["GPIO_OPT_INPUT"]     = "0x" + gpioDefines.GPIO_OPT_INPUT.toString(16);
    output.defines["GPIO_OPT_OUTPUT"]      = "0x" + gpioDefines.GPIO_OPT_OUTPUT.toString(16);
    output.defines["GPIO_OPT_PULLUP"]     = "0x" + gpioDefines.GPIO_OPT_PULLUP.toString(16);
    output.defines["GPIO_OPT_PULLDOWN"]   = "0x" + gpioDefines.GPIO_OPT_PULLDOWN.toString(16);
    output.defines["GPIO_OPT_OPEN_DRAIN"] = "0x" + gpioDefines.GPIO_OPT_OPEN_DRAIN.toString(16);
    output.defines["GPIO_OPT_INVERT"]     = "0x" + gpioDefines.GPIO_OPT_INVERT.toString(16);
    output.defines["GPIO_OPT_DEBOUNCE"]   = "0x" + gpioDefines.GPIO_OPT_DEBOUNCE.toString(16);
}

module.exports.gpioDefines    = gpioDefines;
module.exports.gpioSetDefines = gpioSetDefines;