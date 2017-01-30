assembler             = require('./assembler/IoAssembler');
codes                 = require('./assembler/IoCodes');
configToAssemblerAst  = require('./assembler/IoConfigToAssemblerAst');
configToCompilerAst   = require('./compiler/IoConfigToCompilerAst');


var ioLibrary =
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
Object.defineProperty(ioLibrary, "init", { enumerable: false});

module.exports = ioLibrary;