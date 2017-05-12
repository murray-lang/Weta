/**
 * @fileOverview Take raw IO configuration nodes containing simple objects
 * populated by the parser, and produce nodes suitable for the compiler's
 * abstract syntax tree. Parser expects a standard format for configurations
 * but has no understanding of the semantics. It simply passes the
 * configuration to the identified library (eg this one) for interpretation.
 */
var Types            = require('../../../common/Types');
LibNode              = require('../../../common/LibNode');
ListNode             = require('../../../common/ListNode');
GenericNode          = require('../../../common/GenericNode');
cImmediateNode       = require('../../../compiler/common/AstNodes/ImmediateNode').ImmediateNode;
Token                = require('../../../common/Token');
var gpioDefines      = require('../common/IoGpio').gpioDefines;
var gpioSetDefines   = require('../common/IoGpio').gpioSetDefines;


function compileConfig (token, configObj, formatter)
    {
        var result = { nodes: [], defines: {} };
        for (var nextInterface in configObj)
        {
            if (nextInterface in configCompilers)
            {
                var items = configObj[nextInterface];
                var compiler = configCompilers[nextInterface];
                compiler(token, items, formatter, result);
            }
            else
            {
                formatter.error(
                    false,
                    token,
                    "There is no configuration compiler defined in the '%s' library for interface '%s'",
                    "io",
                    nextInterface);
            }
        }
        return result;
    }

var configCompilers =
{
    digitalin: compileDinConfig,
    gpio: compileGpioConfig,
    pwm: compilePwmConfig,
    adc: compileAdcConfig,
    dac: compileDacConfig,
    servo: compileServoConfig,
    motor: compileMotorConfig,
    shifter: compileShifterConfig,
    stepper: compileStepperConfig
};

function compileImmediate(
    token,
    configObj,
    formatter,
    output,
    name,
    attr,
    type,
    comment)
{
    if (attr in configObj)
    {
        var val = configObj[attr];
        var valNode = new cImmediateNode(
            type,
            new Token(val, token.offset),
            comment
        );
        output.nodes.push(valNode);
    }
    else
    {
        formatter.error(false, token, "No " + attr + " provided for " + name + " configuration.");
    }
}

function compileDinConfig(token, configObj, formatter, output)
{
    if ("select" in configObj)
    {
        var selected = configObj.select;
            // Construct a ListNode by emulating what happens in the
            // high level language parser. ie. Append the list items
            // to a GenericNode and pass that to the ListNode
            // constructor.
        var container = new GenericNode();
        for (var i = 0; i < selected.length; i++)
        {
            var portNode = new cImmediateNode(
                [Types.uint8],
                new Token(selected[i].toString(10), token.offset)
            );
            container.children.push(portNode);
        }
        output.nodes.push(new ListNode(container, [Types.uint8]));

        // Finally, the instruction to invoke the configuration
        var instr = "io.din.config";
        output.nodes.push(new LibNode(new Token(instr, token.offset)));
    }
    else
    {
        formatter.error(false, token, "No ports identified in the digitalin configuration.");
    }

}

function compileCommon(name, token, configObj, formatter, output)
{
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        name,
        "id",
        [Types.uint8],
        "Abstract id of " + name
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        name,
        "io",
        [Types.uint8],
        "Device-specific peripheral id"
    );
}

function compileGpioConfig(token, configObj, formatter, output)
{
    compileCommon("GPIO", token, configObj, formatter, output);
        // Now push the configuration byte for the gpio
    compileGpioParams(token, configObj, formatter, output);
    // Finally, the instruction to invoke the configuration
    var instr = "io.gpio.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));

}

function compileGpioParams(token, configObj, formatter, output)
{
    // Firstly add the required defines
    gpioSetDefines(output);

     // Create an immediate byte for the parameters
    var plus = false;
    var input = true;
    var paramsVal = "";

    if ("out" in configObj) {
        if (configObj["out"].toLowerCase() == "true") {
            paramsVal = "GPIO_OPT_OUTPUT";
            plus = true;
            input = false;
        }
    }
    if ("in" in configObj) {
        if (configObj["in"].toLowerCase() == "true") {
            if (plus)
                paramsVal += " + ";
            paramsVal = "GPIO_OPT_INPUT";
            plus = true;
            input = false;
        }
    }
    if ("pullup" in configObj) {
        if (configObj["pullup"].toLowerCase() == "true") {
            if (plus)
                paramsVal += " + ";
            paramsVal += "GPIO_OPT_PULLUP";
            plus = true;
        }
    }
    if ("pulldown" in configObj) {
        if (configObj["pulldown"].toLowerCase() == "true") {
            if (plus)
                paramsVal += " + ";
            paramsVal += "GPIO_OPT_PULLDOWN";
            plus = true;
        }
    }
    if ("opendrain" in configObj) {
        if (configObj["opendrain"].toLowerCase() == "true") {
            if (plus)
                paramsVal += " + ";
            paramsVal += "GPIO_OPT_OPEN_DRAIN";
            plus = true;
        }
    }
    if ("invert" in configObj) {
        if (configObj["invert"].toLowerCase() == "true") {
            if (plus)
                paramsVal += " + ";
            paramsVal += "GPIO_OPT_INVERT";
            plus = true;
        }
    }
    if (input && "debounce" in configObj) {
        if (configObj["debounce"].toLowerCase() == "true") {
            if (plus)
                paramsVal += " + ";
            paramsVal += "GPIO_OPT_DEBOUNCE";
        }
    }
    if (paramsVal.length == 0)
        paramsVal = "GPIO_OPT_NONE";

    var newParamsNode = new cImmediateNode(
        [Types.uint8],
        new Token(paramsVal, token.offset)
    );
    output.nodes.push(newParamsNode);
}

function compilePwmConfig(token, configObj, formatter, output)
{
    if ("timer" in configObj)
        compilePwmTimerConfig(token, configObj.timer, formatter, output);
    else if ("channel" in configObj)
        compilePwmChannelConfig(token, configObj.channel, formatter, output);
}

function compilePwmTimerConfig(token, configObj, formatter, output)
{
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "PWM Timer",
        "id",
        [Types.uint8],
        "Timer ID"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "PWM Timer",
        "frequency",
        [Types.uint16],
        "Frequency"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "PWM Timer",
        "width",
        [Types.uint8],
        "Width in bits"
    );
        // Finally, the instruction to invoke the configuration
    var instr = "io.pwm.timer.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));
}

function compilePwmChannelConfig(token, configObj, formatter, output)
{
    compileCommon("PWM Channel", token, configObj, formatter, output);
        // Now push the configuration parameters
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "PWM Channel",
        "timer",
        [Types.uint8],
        "Id of PWM timer"
    );

    // Finally, the instruction to invoke the configuration
    var instr = "io.pwm.channel.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));
}

function compileAdcConfig(token, configObj, formatter, output)
{
        // if there is a "depth" attribute then this is a configuration for
        // ADC as a whole
    if ("depth" in configObj)
    {
        compileImmediate(
            token,
            configObj,
            formatter,
            output,
            "ADC",
            "depth",
            [Types.uint8],
            "Depth in bits"
        );
        // The instruction to invoke the configuration
        var instr = "io.adc.config";
        output.nodes.push(new LibNode(new Token(instr, token.offset)));
    }
        // Any "channel" attribute is an object containing the configuration
        // for a particular channel only.
    if ("channel" in configObj)
    {
        compileAdcChannelConfig(token, configObj["channel"], formatter, output)
    }
}

function compileAdcChannelConfig(token, configObj, formatter, output) {
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "ADC",
        "id",
        [Types.uint8],
        "Abstract id of ADC channel"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "ADC",
        "gain",
        [Types.int8],
        "Gain in dB (attenuation if negative)"
    );
    // Finally, the instruction to invoke the configuration
    var instr = "io.adc.channel.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));
}

function compileDacConfig(token, configObj, formatter, output)
{
    compileCommon("DAC", token, configObj, formatter, output);
         // Now push the configuration parameters
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "DAC",
        "depth",
        [Types.uint8],
        "Depth in bits"
    );
        // Finally, the instruction to invoke the configuration
    var instr = "io.dac.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));

}

function compileServoConfig(token, configObj, formatter, output)
{
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Servo",
        "id",
        [Types.uint8],
        "Abstract id of Servo"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Servo",
        "pwm",
        [Types.uint8],
        "Abstract id of PWM to use"
    );

    // Now push the configuration parameters
    compileServoParams(token, configObj, formatter, output);
    // Finally, the instruction to invoke the configuration
    var instr = "io.servo.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));
}

function compileServoParams(token, configObj, formatter, output)
{
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Servo",
        "minduty",
        [Types.uint16],
        "Minimum duty"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Servo",
        "maxduty",
        [Types.uint16],
        "Maximum duty"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Servo",
        "duty0",
        [Types.uint16],
        "Duty when centred"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Servo",
        "dutyper10",
        [Types.int16],  // Note signed
        "Duty change per 10 degrees"
    );
}

function compileMotorConfig(token, configObj, formatter, output)
{
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Motor",
        "id",
        [Types.uint8],
        "Abstract id of motor"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Motor",
        "pwm",
        [Types.uint8],
        "PWM id"
    );

    if ("dir" in configObj)
    {
        var dir = configObj.dir;
        var dirNode = new cImmediateNode(
            [Types.uint8],
            new Token(dir, token.offset),
            "GPIO id for direction"
        );
        output.nodes.push(dirNode);
            // Create a define for invalid GPIO#
        output.defines["GPIO_INVALID"] = "0xFF";
        var ignoreNode = new cImmediateNode(
            [Types.uint8],
            new Token("GPIO_INVALID", token.offset),
            "This will flag that the first GPI0# is for direction"
        );
        output.nodes.push(ignoreNode);
    }
    else if ("a" in configObj && "b" in configObj)
    {
        var a = configObj.a;
        var aNode = new cImmediateNode(
            [Types.uint8],
            new Token(a, token.offset),
            "GPIO id for H-bridge A"
        );
        output.nodes.push(aNode);

        var b = configObj.b;
        var bNode = new cImmediateNode(
            [Types.uint8],
            new Token(b, token.offset),
            "GPIO id for H-bridge B"
        );
        output.nodes.push(bNode);
    }
    else
    {
        formatter.error(false, token, "Insufficient motor configuration.");
        return;
    }

        // Finally, the instruction to invoke the configuration
    var instr = "io.motor.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));
}

function compileShifterConfig(token, configObj, formatter, output) {
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Shifter",
        "id",
        [Types.uint8],
        "Abstract id of shift register"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Shifter",
        "width",
        [Types.uint8],
        "Number of bits"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Shifter",
        "data",
        [Types.uint8],
        "Abstract GPIO# for data"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Shifter",
        "clock",
        [Types.uint8],
        "Abstract GPIO# for clock"
    );
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Shifter",
        "strobe",
        [Types.uint8],
        "Abstract GPIO# for strobe"
    );
        // Finally, the instruction to invoke the configuration
    var instr = "io.shifter.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));
}

function compileStepperConfig(token, configObj, formatter, output)
{
    compileImmediate(
        token,
        configObj,
        formatter,
        output,
        "Stepper",
        "id",
        [Types.uint8],
        "Abstract id of stepper motor"
    );
    if ("reverse" in configObj) {
        compileImmediate(
            token,
            configObj,
            formatter,
            output,
            "Stepper",
            "reverse",
            [Types.bool],
            "Motor is mounted backwards?"
        );
    } else {
            // Default to false
        var falseNode = new cImmediateNode(
            [Types.bool],
            new Token("false", token.offset),
            "Motor is not mounted backwards"
        );
        output.nodes.push(falseNode);
    }
    if ("shifter" in configObj)
    {
        compileImmediate(
            token,
            configObj,
            formatter,
            output,
            "Stepper",
            "shifter",
            [Types.uint8],
            "Abstract id of shifter"
        );
        output.defines["GPIO_INVALID"] = "0xFF";
            // An invalid second parameter flags the use of a shifter
        var ignoreNode = new cImmediateNode(
            [Types.uint8],
            new Token("GPIO_INVALID", token.offset),
            "Invalid here means shifter in use"
        );
        output.nodes.push(ignoreNode);
            // if an offset is provided then use it otherwise 0
        if ("offset" in configObj) {
            compileImmediate(
                token,
                configObj,
                formatter,
                output,
                "Stepper",
                "offset",
                [Types.uint8],
                "Offset within shifter"
            );
        }
        else
        {
            var zeroNode = new cImmediateNode(
                [Types.uint8],
                new Token(0, token.offset),
                "Offset within shifter"
            );
            output.nodes.push(zeroNode);
        }
        ignoreNode = new cImmediateNode(
            [Types.uint8],
            new Token("GPIO_INVALID", token.offset),
            "Padding for unrequired 'D' GPIO"
        );
        output.nodes.push(ignoreNode);
    }
    else if ("a" in configObj && "b" in configObj)
    {
        compileImmediate(
            token,
            configObj,
            formatter,
            output,
            "Stepper",
            "a",
            [Types.uint8],
            "Device-specific GPIO# for A"
        );
        compileImmediate(
            token,
            configObj,
            formatter,
            output,
            "Stepper",
            "b",
            [Types.uint8],
            "Device-specific GPIO# for B"
        );
        if ("c" in configObj && "d" in configObj)
        {
            compileImmediate(
                token,
                configObj,
                formatter,
                output,
                "Stepper",
                "c",
                [Types.uint8],
                "Device-specific GPIO# for C"
            );
            compileImmediate(
                token,
                configObj,
                formatter,
                output,
                "Stepper",
                "d",
                [Types.uint8],
                "Device-specific GPIO# for D"
            );
        }
        else
        {
            output.defines["GPIO_INVALID"] = "0xFF";
            var i;
            for (i = 0; i < 2; i++)
            {
                var ignoreNode = new cImmediateNode(
                    [Types.uint8],
                    new Token("GPIO_INVALID", token.offset),
                    "Ignore this parameter"
                );
                output.nodes.push(ignoreNode);
            }
        }
    }
    else
    {
        formatter.error(false, token, "Insufficient stepper configuration.");
        return;
    }

    // Finally, the instruction to invoke the configuration
    var instr = "io.stepper.config";
    output.nodes.push(new LibNode(new Token(instr, token.offset)));
}

module.exports = compileConfig;