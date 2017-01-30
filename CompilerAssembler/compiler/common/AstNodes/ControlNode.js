CompileNodeType      = require("./NodeType");
ImmediateNode        = require("./ImmediateNode").ImmediateNode;
ExpressionNode       = require("./ExpressionNode").ExpressionNode;
ExpressionNodeKind   = require("./ExpressionNode").ExpressionNodeKind;
var Types            = require("../../../common/Types");
var VariableNodeKind = require('./VariableNode').VariableNodeKind;
var VariableNode     = require('./VariableNode').VariableNode;

var ControlNodeKind =
{
    "if":        { toString: function () { return "if"; } },
    "ifelse":    { toString: function () { return "ifelse"; } },
    "repeat":    { toString: function () { return "repeat"; } },
    "foreach":   { toString: function () { return "foreach"; } },
    "forever":   { toString: function () { return "forever"; } },
    "for":       { toString: function () { return "for"; } },
    "while":     { toString: function () { return "while"; } },
    "dowhile":   { toString: function () { return "dowhile"; } },
    "tag":       { toString: function () { return "tag"; } },
    "goto":      { toString: function () { return "goto"; } },
    "output":    { toString: function () { return "output"; } },
    "return":    { toString: function () { return "return"; } },
    "wait":      { toString: function () { return "wait"; } },
    "waitms":    { toString: function () { return "waitms"; } },
    "waituntil": { toString: function () { return "waituntil"; } }
};

function ControlNode(kind, token, children)
{
    this.nodeType   = CompileNodeType.control;
    this.token      = token;
    this.resultType = kind == ControlNodeKind.output ? [Types.unknown] : [Types.void];
    this.kind       = kind;
    this.children   = [];

    if (kind == ControlNodeKind.for)
    {
        this.children.push(new VariableNode(arguments[2], VariableNodeKind.nameof, [Types.int16]));
        for (var i = 3; i < arguments.length; i++)
            this.children.push(arguments[i]);
    }
    else if (kind == ControlNodeKind.foreach)
    {
        var iterator = new VariableNode(
            arguments[2],
            VariableNodeKind.iterator,
            [Types.unknown]
        );
        this.children.push(iterator);
        for (var i = 3; i < arguments.length; i++)
            this.children.push(arguments[i]);
    }
    else if (kind == ControlNodeKind.wait)
    {
        // In Cricket Logo, wait takes an argument specified in 10ths of a
        // second. However the virtual machine back end only has a wait
        // command specified in milliseconds. Therefore, add commands to
        // multiply the argument to wait by 100.
        var multiplier = new ImmediateNode(
                [Types.int16],
                { value: 100, match: "100", offset: token.offset}
        );
        var exp = new ExpressionNode(
            token,
            ExpressionNodeKind.math,
            "mul",
            [Types.int16],
            arguments[2],
            multiplier
        );
        this.children.push(exp);
    }
    else
    {
        for (var i = 2; i < arguments.length; i++)
            this.children.push(arguments[i]);
    }
}

module.exports.ControlNode     = ControlNode;
module.exports.ControlNodeKind = ControlNodeKind;