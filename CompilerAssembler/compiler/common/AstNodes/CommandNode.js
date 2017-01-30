CompileNodeType     = require("./NodeType");
var Types    = require("../../../common/Types");

var CommandNodeKind =
    {
        "resett":       { toString: function () { return "resett"; } },
        "setsvh":       { toString: function () { return "setsvh"; } },
        "svr":          { toString: function () { return "svr"; } },
        "svl":          { toString: function () { return "svl"; } },
        "resetdp":      { toString: function () { return "resetdp"; } },
        "setdp":        { toString: function () { return "setdp"; } },
        "record":       { toString: function () { return "record"; } },
        "erase":        { toString: function () { return "erase"; } },
        "send":         { toString: function () { return "send"; } },
        "receive":      { toString: function () { return "receive"; } },
        "received":     { toString: function () { return "received"; } },
        "digitalout":   { toString: function () { return "digitalout"; } },
        "analogout":    { toString: function () { return "analogout"; } },
        "ledon":        { toString: function () { return "ledon"; } },
        "ledoff":       { toString: function () { return "ledoff"; } },
        "beep":         { toString: function () { return "beep"; } },
        "i2c":          { toString: function () { return "i2c"; } },
        "on":           { toString: function () { return "on"; } },
        "onfor":        { toString: function () { return "onfor"; } },
        "off":          { toString: function () { return "off"; } },
        "thisway":      { toString: function () { return "thisway"; } },
        "thatway":      { toString: function () { return "thatway"; } },
        "rd":           { toString: function () { return "rd"; } },
        "brake":        { toString: function () { return "brake"; } },
        "setpower":     { toString: function () { return "setpower"; } },
        "encode":       { toString: function () { return "encode"; } },
        "decode":       { toString: function () { return "decode"; } },
        "forward":      { toString: function () { return "forward"; } },
        "backward":     { toString: function () { return "backward"; } },
        "left":         { toString: function () { return "left"; } },
        "right":        { toString: function () { return "right"; } },
        "penup":        { toString: function () { return "penup"; } },
        "pendown":      { toString: function () { return "pendown"; } }
    };

function CommandNode(kind, token, children)
    {
        this.nodeType   = CompileNodeType.command;
        this.token      = token;
        this.resultType = [Types.void];
        this.kind       = kind;
        this.children   = [];

        if (kind == CommandNodeKind.send && arguments.length > 3)
        {
                // If the send command has more than 1 argument then it's the
                // version that sends an array. In this case the number of
                // bytes sent is returned.
            this.resultType = [Types.uint8];
        }
        else if (kind == CommandNodeKind.receive)
        {
                // If receive has no argument then the result type will be
                // determined by the left hand side of the expression.
                // Mark the type as unknown so that heuristics will be used
                // in the processing of the AST.
                // If the receive has more than 1 argument then it's the
                // version that receives an array. In this case the number of
                // bytes received is returned.
            if (arguments.length == 2)
                this.resultType = [Types.unknown];
            else if (arguments.length > 2)
                this.resultType = [Types.uint8];
        }
        else if (kind == CommandNodeKind.received)
        {
            this.resultType = [Types.bool];
        }
        else if (kind == CommandNodeKind.i2c)
        {
            this.command = this.token.value;
        }
        else if (kind == CommandNodeKind.encode)
        {
            this.resultType = [Types.list(0), Types.uint8];
                // The size of the input array is the second argument to encode
                // (which is the fourth argument of this function).
            this.inputType = [Types.array(arguments[3].value), Types.unknown];
        }
        else if (kind == CommandNodeKind.decode)
        {
                // Determine the list item type from the third argument.
                // String type information is obtained through a method, so
                // treat it as a special case.
            var decodeType;
            if (arguments[2].value == "string")
                decodeType = Types.string(0);
            else
                decodeType = Types[arguments[2].value];
                // Use the decode type as a direct key into the types
            this.resultType = [Types.list(0), decodeType];
            this.inputType = [Types.array(0), Types.uint8];
        }

        for (var i = 2; i < arguments.length; i++)
            this.children.push(arguments[i]);
    }

module.exports.CommandNode     = CommandNode;
module.exports.CommandNodeKind = CommandNodeKind;