commLib          = require('./comm/CommLib');
ioLib            = require('./io/IoLib');
mathLib          = require('./math/MathLib');
AddTokenOffset   = require('../common/traverse/AddTokenOffset');


var libraries =
    {
        comm: commLib,
        io:   ioLib,
        math: mathLib,

        init: function()
                {
                    for (var libName in this)
                    {
                        this[libName].codes.init();
                    }
                },

        /**
         * Parse an assembly language statement that has the dot format
         * indicating that it is a library call.
         * @param token
         * @param formatter
         * @returns {*} A GenericNode object with the assembler AST nodes as children
         */
        assemble: function (token, formatter)
                {
                    var dotIndex = token.value.indexOf('.');

                    if (dotIndex == -1)
                    {
                        formatter.error(false, token, "Internal error: Library call '%s' has no '.' delimiter", libName);
                        return null;
                    }
                    var libName = token.value.slice(0, dotIndex);
                    var text    = token.value.slice(dotIndex+1);
                    if (this[libName] === undefined)
                    {
                        formatter.error(false, token, "Library '%s' not found", libName);
                        return null;
                    }
                    var library = this[libName];
                    library.assembler.ast.init(); // Cleanup any remnants from a previous parse
                    var error_off	= [];
                    var error_la	= [];
                    var error_cnt = library.assembler.parse(text + '\n', error_off, error_la);
                    if( error_cnt > 0 )
                    {
                        for( var i = 0; i < error_cnt; i++ )
                        {
                            formatter.rawError(
                                "Parse error near >%s<, expecting '%s'",
                                text.substr( error_off[i], 30 ),
                                error_la[i].join()
                            );
                        }
                        return null;
                    }
                    else
                    {
                            // Return the GenericNode at the root of the AST.
                            // But first convert its token offsets to absolute.
                        new AddTokenOffset(formatter, token.offset).traverse(library.assembler.ast.nodes);
                        return library.assembler.ast.nodes;
                    }
                }

    };

// Prevent the init function being enumerated as a library
Object.defineProperty(libraries, "init", { enumerable: false});
Object.defineProperty(libraries, "assemble", { enumerable: false});

module.exports = libraries;