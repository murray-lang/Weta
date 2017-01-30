AstTraverser   = require('../../../common/AstTraverser');
AddTokenOffset = require('../../../common/traverse/AddTokenOffset');
//var libraries    = require('../../../lib/Libraries');

function ParseLibraryCalls(formatter, libraries)
{
    this.formatter = formatter;
    this.libraries = libraries;
}
ParseLibraryCalls.prototype = new AstTraverser();
ParseLibraryCalls.prototype.constructor = ParseLibraryCalls;

ParseLibraryCalls.prototype.default =
    function (node)
    {
        this.traverseChildren(node);
    };

ParseLibraryCalls.prototype["lib"] =
    function (node)
    {
        var containerNode = this.libraries.assemble(node.token, this.formatter);

             // Copy the children from the GenericNode in the AST to this node
            //node.children.push.apply(node.children, library.parser.ast.nodes.children);
        for (var i = 0; i < containerNode.children.length; i++)
        {
            var nextChild = containerNode.children[i];
            nextChild.token = node.token;
            node.children.push(nextChild);
        }

    };

module.exports = ParseLibraryCalls;