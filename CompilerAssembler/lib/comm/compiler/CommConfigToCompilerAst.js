var Types            = require('../../../common/Types');
LibNode              = require('../../../common/LibNode');
var serialDefines    = require('../common/CommSerial').serialDefines;
var serialSetDefines = require('../common/CommSerial').serialSetDefines;
cImmediateNode       = require('../../../compiler/common/AstNodes/ImmediateNode').ImmediateNode;
Token                = require('../../../common/Token');

function compileConfig (token, configObj, formatter)
    {
        var result = { nodes: [], defines: {} };
        for (var nextInterface in configObj)
        {
            if (nextInterface in configCompilers)
            {
                var items = configObj[nextInterface];
                var configAssembler = configCompilers[nextInterface];
                configAssembler(token, items, formatter, result);
            }
            else
            {
                formatter.error(
                    false,
                    token,
                    "There is no configuration compiler defined in the '%s' library for interface '%s'",
                    "comm",
                    nextInterface);
            }
        }
        return result;
    }

var configCompilers =
{
    serial: compileSerialConfig
};

function compileSerialConfig(token, configObj, formatter, output)
    {
        if ("port" in configObj)
        {
            var portNum = configObj.port;
            // The serial port is identified by a number between 0 and 255.
            // Create a node to push the port number onto the stack.
            var portNode = new cImmediateNode(
                [Types.uint8],
                new Token(portNum, token.offset)
            );
            output.nodes.push(portNode);
            // Now push the other parameters
            compileSerialParams(token, configObj, formatter, output);
            // Finally, the instruction to invoke the configuration
            var instr = "comm.serial.config";
            output.nodes.push(new LibNode(new Token(instr, token.offset)));
        }
        else
        {
            formatter.error(false, token, "No port identified in the serial port configuration.");
        }

    }

function compileSerialParams(token, configObj, formatter, output)
    {
        serialSetDefines(output);

        // Set the new rate if it's given
        var baud;
        if ("baud" in configObj)
            baud = configObj["baud"];
        else
            baud = "9600";
        // Create an immediate 32 bit int for the baud (9600 default)
        var baudNode = new cImmediateNode(
            [Types.uint32],
            new Token(baud, token.offset)
        );
        output.nodes.push(baudNode);

        // Create an immediate byte for the parameters
        var paramsVal = "";
        if ("databits" in configObj)
            paramsVal = "DATABITS_" + configObj["databits"];
        else
            paramsVal = "DATABITS_8";

        paramsVal += " + ";
        if ("parity" in configObj)
            paramsVal += "PARITY_" + configObj["parity"].toUpperCase();
        else
            paramsVal += "PARITY_NONE";

        paramsVal += " + ";
        if ("stopbits" in configObj)
            paramsVal += "STOPBITS_" + configObj["stopbits"];
        else
            paramsVal += "STOPBITS_1";

        var newParamsNode = new cImmediateNode(
            [Types.uint8],
            new Token(paramsVal, token.offset)
        );
        output.nodes.push(newParamsNode);
    }

module.exports = compileConfig;