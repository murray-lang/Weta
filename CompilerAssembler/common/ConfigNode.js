
var ConfigNodeSource =
    {
        "compiler":       { toString: function () { return "compiler"; } },
        "assembler":       { toString: function () { return "assembler"; } }
    };

function ConfigNode(token, source, library, configObj)
{
    this.nodeType  = "config";
    this.token     = token;
    this.source    = source;
    this.library   = library;
    this.configObj = configObj;
}

module.exports.ConfigNode       = ConfigNode;
module.exports.ConfigNodeSource = ConfigNodeSource;

