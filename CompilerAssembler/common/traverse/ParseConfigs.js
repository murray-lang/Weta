var AstTraverser  = require('../AstTraverser');
var Types         = require('../Types');
var ConfigNodeSource = require('../ConfigNode').ConfigNodeSource;

//CompilerNodeType          = require('../../../common/AstNodes/NodeType');
//var ImmediateNode  = require('../../compiler/common/AstNodes/ImmediateNode').ImmediateNode;

function ParseConfigs(formatter, libraries)
{
    this.formatter = formatter;
    this.libraries = libraries;
}

ParseConfigs.prototype = new AstTraverser();
ParseConfigs.prototype.constructor = ParseConfigs;

ParseConfigs.prototype.default =
    function (node)
    {
        this.traverseChildren(node);
    };

// Parsing functions are keyed by 'item'
ParseConfigs.prototype.parsers = {};
ParseConfigs.prototype.parsers.send = {};

ParseConfigs.prototype["config"] =
    function (node)
    {
        if (!(node.library in this.libraries))
        {
            this.formatter.error(false, node.token, "Library '%s' not found", node.library);
            return;
        }
        var library = this.libraries[node.library];
        var nodesAndDefines;
        if (node.source == ConfigNodeSource.compiler)
        {
            nodesAndDefines = library.configToCompilerAst(node.token, node.configObj, this.formatter);
            node.children = nodesAndDefines.nodes;
            node.defines  = nodesAndDefines.defines;
        }
        else
        {
                // Assume assembler
            nodesAndDefines = library.configToAssemblerAst(node.token, node.configObj, this.formatter);
            node.children = nodesAndDefines.nodes;
            node.defines  = nodesAndDefines.defines;
        }
    };


ParseConfigs.prototype.parsers.send.parse =
    function (node, formatter)
    {
        // Declare the symbols required for send configuration
        this.setDefines(node);
            // The second (and last) parameter is the abstract port number
        var portNode = node.children.pop();
            // The first parameter is a token containing either
            // "serial" or "ethernet".
        var transportToken = node.children.pop();

            // Push the port node straight back onto the children as-is
        node.children.push(portNode);
            // The transport selection now goes at the end, meaning that it
            // gets emitted last (and pushed last, and popped first in the
            // target code that handles it)
        var transportNode = new ImmediateNode([Types.uint8]);
        if (transportToken.value == "serial")
            transportNode.value = "SEND_SERIAL";
        else if (transportToken.value == "ethernet")
            transportNode.value = "SEND_ETHERNET";
        else
        {
            formatter.error(
                false,
                transportToken,
                "'%s' is not a valid transport for configuration of 'send'. Defaulting to serial.",
                transportToken.value
            );
            transportNode.value = "SEND_SERIAL";
        }
        node.children.push(transportNode);
    };

ParseConfigs.prototype.parsers.send.setDefines =
    function(node)
    {
        // Databits, parity and stop bits are packed into a byte as fields
        node.defines =
        {
            "SEND_SERIAL":   "0",
            "SEND_ETHERNET": "1"
        };

    };


module.exports = ParseConfigs;
//module.exports.Processing   = Processing;