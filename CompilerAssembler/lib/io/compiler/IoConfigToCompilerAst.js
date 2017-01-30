var Types            = require('../../../common/Types');
LibNode              = require('../../../common/LibNode');
ListNode             = require('../../../common/ListNode');
GenericNode          = require('../../../common/GenericNode');
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
    digitalin: compileDinConfig
};

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

module.exports = compileConfig;