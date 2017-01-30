var serialDefines = {
    DATABITS_5:  5 << 4,
    DATABITS_6:  6 << 4,
    DATABITS_7:  7 << 4,
    DATABITS_8:  8 << 4,
    PARITY_NONE: 0 /* << 2 */,
    PARITY_ODD:  1 << 2,
    PARITY_EVEN: 2 << 2,
    STOPBITS_1:  1,
    STOPBITS_2:  2
};

function serialSetDefines(output)
{
    // Databits, parity and stop bits are packed into a byte as fields
    output.defines["DATABITS_5"]  = "0x" + serialDefines.DATABITS_5.toString(16);
    output.defines["DATABITS_6"]  = "0x" + serialDefines.DATABITS_6.toString(16);
    output.defines["DATABITS_7"]  = "0x" + serialDefines.DATABITS_7.toString(16);
    output.defines["DATABITS_8"]  = "0x" + serialDefines.DATABITS_8.toString(16);
    output.defines["PARITY_NONE"] = "0x" + serialDefines.PARITY_NONE.toString(16);
    output.defines["PARITY_ODD"]  = "0x" + serialDefines.PARITY_ODD.toString(16);
    output.defines["PARITY_EVEN"] = "0x" + serialDefines.PARITY_EVEN.toString(16);
    output.defines["STOPBITS_1"]  = "0x" + serialDefines.STOPBITS_1.toString(16);
    output.defines["STOPBITS_2"]  = "0x" + serialDefines.STOPBITS_2.toString(16);
}

module.exports.serialDefines    = serialDefines;
module.exports.serialSetDefines = serialSetDefines;