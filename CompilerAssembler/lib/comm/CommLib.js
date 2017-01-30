assembler             = require('./assembler/CommAssembler');
codes                 = require('./assembler/CommCodes');
configToAssemblerAst  = require('./assembler/CommConfigToAssemblerAst');
configToCompilerAst   = require('./compiler/CommConfigToCompilerAst');


var commLibrary =
    {
            // Assembles instructions into byte codes
        assembler:      assembler,
            // The byte code lookup table
        codes:          codes,
             // Converts a configuration object into compiler AST nodes
        configToCompilerAst:  configToCompilerAst,
            // Converts a configuration object into assembler AST nodes
        configToAssemblerAst: configToAssemblerAst,

        init: function()
        {
            this.codes.init();
        }
    };

// Prevent the init function being enumerated as a library
Object.defineProperty(commLibrary, "init", { enumerable: false});

module.exports = commLibrary;