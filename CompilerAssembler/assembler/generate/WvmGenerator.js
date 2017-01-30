var AstTraverser = require('../../common/AstTraverser');
var Scope        = require('../../common/Scope');
//var wvmCodes     = require('./codes/WvmCodes');
OptimiseDataInitialisation = require('./traverse/OptimiseDataInitialisation');
ResolveSymbolReferences    = require('./traverse/ResolveSymbolReferences');
Assemble                   = require('./traverse/Assemble');

function WvmGenerator (formatter, libraries)
{
    this.formatter = formatter;
    this.libraries = libraries;
    this.alignment = 2;
}
WvmGenerator.prototype = new AstTraverser();
WvmGenerator.prototype.constructor = WvmGenerator;

WvmGenerator.prototype.generate =
    function (ast)
    {
        this.prepare(ast);

        var context =
            {
                origin:    0,
                alignment: [this.alignment], // Alignments can be pushed and popped
                globals:   { labels: {}, variables: {}, variablesCursor: 0 },
                procs:     {},
                blocks:    [],  // Blocks are remembered here to match up with eobs
                codes:     []   // Assembled machine codes go here
            };
        new Assemble(this.formatter, this.libraries).traverse(ast.nodes, context, Scope.global);
        new ResolveSymbolReferences(this.formatter).traverse(ast.nodes, context);

        var result = {};
        result.address = context.origin;
        result.codes = context.codes;
        result.errors = this.formatter.errors;
        result.warnings = this.formatter.warnings;
            // Symbols etc. will be added to the output object once I've worked
            // out how I'm going to use them.
        return result;
    };

WvmGenerator.prototype.prepare =
    function (ast)
    {
        new OptimiseDataInitialisation().traverse(ast.nodes);
    };

module.exports = WvmGenerator;
