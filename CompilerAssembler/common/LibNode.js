

function LibNode(token, children)
{
    this.nodeType = "lib";
    this.token    = token;
    this.children = [];

    for( var i = 1; i < arguments.length; i++ )
        this.children.push( arguments[i] );
}

module.exports = LibNode;
