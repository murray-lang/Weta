var Types = require('./Types');

function ListNode(items, type)
{
    this.nodeType = "list";
    if (type === undefined)
    {
        this.resultType = [Types.list(items.children.length), Types.unknown];
    }
    else
    {
        this.resultType = [Types.list(items.children.length)].concat(type);
    }
    this.children   = items.children;
}

module.exports = ListNode;