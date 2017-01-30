AstTraverser = require('../AstTraverser');

/**
 * Add an offset to all of the offsets of tokens in the nodes of the tree.
 * This is used to convert offsets in separately parsed fragments to their
 * offsets in the text as a whole.
 * @param formatter
 * @param offset
 * @constructor
 */
function AddTokenOffset(formatter, offset)
{
    this.formatter = formatter;
    this.offset = offset;
}
AddTokenOffset.prototype = new AstTraverser();
AddTokenOffset.prototype.constructor = AddTokenOffset;

AddTokenOffset.prototype.default =
    function (node)
    {
        if (node.token !== undefined)
            node.token.offset += this.offset;

        this.traverseChildren(node);
    };

module.exports = AddTokenOffset;