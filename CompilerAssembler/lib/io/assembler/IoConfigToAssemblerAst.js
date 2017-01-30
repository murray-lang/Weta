var Types            = require('../../../common/Types');
Token                = require('../../../common/Token');
asImmediateNode      = require('../../../assembler/AstNodes/AsmImmediateNode');
ListNode             = require('../../../common/ListNode');
GenericNode          = require('../../../common/GenericNode');
DataNode             = require('../../../assembler/AstNodes/DataNode');
LibNode              = require('../../../common/LibNode');

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
    din: assembleDinConfig
};

function assembleDinConfig(token, configObj, formatter, output)
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
            var portNode = new DataNode(
                new Token("uint8", token.offset),
                new asImmediateNode(new Token(selected[i].toString(10), token.offset ))
            );
            container.children.push(portNode);
        }
        output.nodes.push(new ListNode(container, [Types.uint8]));

        var instr = "io.din.config";
        output.nodes.push(new LibNode(new Token(instr, token.offset)));
    }
    else
    {
        formatter.error(false, token, "No ports identified in the digitalin configuration.");
    }

}

module.exports = assembleConfig;