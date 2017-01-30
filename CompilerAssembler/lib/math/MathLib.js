assembler             = require('./assembler/MathAssembler');
codes                 = require('./assembler/MathCodes');

var mathLibrary =
    {
            // Assembles instructions into byte codes
        assembler:      assembler,
            // The byte code lookup table
        codes:          codes,
             // Converts a configuration object into compiler AST nodes
        configToCompilerAst:  null,
            // Converts a configuration object into assembler AST nodes
        configToAssemblerAst: null,

        init: function()
        {
            this.codes.init();
        }
    };

// Prevent the init function being enumerated as a library
Object.defineProperty(mathLibrary, "init", { enumerable: false});

module.exports = mathLibrary;