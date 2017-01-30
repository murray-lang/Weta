AssembleNodeType = require('./NodeType');

function AsmImmediateNode(token)
{
    this.nodeType = AssembleNodeType.immediate;
    this.token    = token;
    this.value    = token.value;
}

module.exports = AsmImmediateNode;