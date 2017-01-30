//var Types           = require('../../../common/Types');
Token                = require('../../../common/Token');
asImmediateNode      = require('../../../assembler/AstNodes/AsmImmediateNode');
DataNode             = require('../../../assembler/AstNodes/DataNode');
LibNode              = require('../../../common/LibNode');
var serialDefines    = require('../common/CommSerial').serialDefines;
var serialSetDefines = require('../common/CommSerial').serialSetDefines;
/**
 * Generate assembler Abstract Syntax Tree instruction nodes based on the given
 * configuration object.
 * @param token Token parsed from source text
 * @param configObj An object containing configuration information in a format
 *          specific to this library. The only limitation is that it must be
 *          able to be represented in JSON.
 * @param formatter Object used to format output messages
 */
function assembleConfig (token, configObj, formatter)
{
    var result = { nodes: [], defines: {} };
    for (var nextInterface in configObj)
    {
        if (nextInterface in configAssemblers)
        {
            var items = configObj[nextInterface];
            var configAssembler = configAssemblers[nextInterface];
            configAssembler(token, items, formatter, result);
        }
        else
        {
            formatter.error(
                false,
                token,
                "There is no configuration assembler defined in the '%s' library for interface '%s'",
                "comm",
                nextInterface);
        }
    }
    return result;
}

var configAssemblers =
{
    serial: assembleSerialConfig,
    wifi:   assembleWifiConfig
};

function assembleSerialConfig(token, configObj, formatter, output)
{
    if ("port" in configObj)
    {
        var portNum = configObj.port;
            // The serial port is identified by a number between 0 and 255.
            // Create a node to push the port number onto the stack.
        var portNode = new DataNode(
            new Token("uint8", token.offset),
            new asImmediateNode(new Token(portNum, token.offset ))
        );
        output.nodes.push(portNode);
            // Now push the other parameters
        assembleSerialParams(token, configObj, formatter, output);
            // Finally, the instruction to invoke the configuration
        var instr = "comm.serial.config";
        output.nodes.push(new LibNode(new Token(instr, token.offset)));
    }
    else
    {
        formatter.error(false, token, "No port identified in the serial port configuration.");
    }

}

function assembleSerialParams(token, configObj, formatter, output)
{
    serialSetDefines(output);

        // Set the new rate if it's given
    var baud;
    if ("baud" in configObj)
        baud = configObj["baud"];
    else
        baud = "9600";
        // Create an immediate 32 bit int for the baud (9600 default)
    var baudNode = new DataNode(
        new Token("uint32", token.offset),
        new asImmediateNode(new Token(baud, token.offset))
    );
    output.nodes.push(baudNode);

        // Create an immediate byte for the parameters
    var paramsVal = 0;
    if ("databits" in configObj)
        paramsVal = serialDefines["DATABITS_" + configObj["databits"]];
    else
        paramsVal = serialDefines.DATABITS_8;

    if ("parity" in configObj)
        paramsVal += serialDefines["PARITY_" + configObj["parity"].toUpperCase()];
    else
        paramsVal += serialDefines.PARITY_NONE;

    if ("stopbits" in configObj)
        paramsVal += serialDefines["STOPBITS_" + configObj["stopbits"]];
    else
        paramsVal += serialDefines.STOPBITS_1;

    var newParamsNode = new DataNode(
        new Token("uint8", token.offset),
        new asImmediateNode(new Token(paramsVal.toString(10), token.offset))
    );
    newParamsNode.value = paramsVal;
    output.nodes.push(newParamsNode);
}



var wifiItemAssemblers =
{
    ip:      wifiIpConfigAssembler,
    dns:     wifiDnsConfigAssembler,
    gateway: wifiGatewayConfigAssembler,
    mask:    wifiMaskConfigAssembler
};


function assembleWifiConfig(token, configObj, formatter, output)
{
    for (item in configObj)
    {
        if (item in wifiItemAssemblers)
        {
            var itemAssembler = wifiItemAssemblers[item];
            var itemConfig = configObj[item];
            itemAssembler(itemConfig, output);
        }
        else
        {

        }
    }
}

function wifiIpConfigAssembler(token, configObj, formatter, output)
{

}

function wifiDnsConfigAssembler(token, configObj, formatter, output)
{

}

function wifiGatewayConfigAssembler(token, configObj, output)
{

}

function wifiMaskConfigAssembler(token, configObj, output)
{

}

module.exports = assembleConfig;