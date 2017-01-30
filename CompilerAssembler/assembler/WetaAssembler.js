/*
        Default driver template for JS/CC generated parsers for V8
        
        Features:
        - Parser trace messages
        - Step-by-step parsing
        - Integrated panic-mode error recovery
        - Pseudo-graphical parse tree generation
        
        Written 2007 by Jan Max Meyer, J.M.K S.F. Software Technologies
        Modified 2008 from driver.js_ to support V8 by Louis P.Santillan
                        <lpsantil@gmail.com>
        
        This is in the public domain.
*/


//--------------------------------------------------------------------------
// My stuff

var parser                = require('./WasmParser');
var AssemblerAstProcessor = require('./process/AssemblerAstProcessor');
var WvmGenerator          = require('./generate/WvmGenerator');
var wvmCodes              = require('./generate/codes/WvmCodes');
var libraries             = require('../lib/Libraries');

function WetaAssembler(messageFormatter)
{
    this.formatter = messageFormatter;
    parser.ast.setFormatter(messageFormatter);
}

WetaAssembler.prototype.parse =
    function (text)
    {
        var error_off	= [];
        var error_la	= [];

        //LogoCC_dbg_withparsetree = true;
        //LogoCC_dbg_withtrace = true;
        var error_cnt = parser.parse( text, error_off, error_la );
        if( error_cnt > 0 )
        {
            for( var i = 0; i < error_cnt; i++ )
            {
                this.formatter.rawError(
                    "Parse error near >%s<, expecting '%s'",
                    text.substr( error_off[i], 30 ),
                    error_la[i].join()
                );
            }

        }
        return error_cnt;
    };

WetaAssembler.prototype.assemble =
    function (text)
    {
        // Wrap the formatter such that each call has the source text
        // prepended to the arguments.
        var formatter =
            {
                debug: this.formatter.debug.bind(this.formatter, text),
                info:  this.formatter.info.bind(this.formatter, text),
                warn:  this.formatter.warn.bind(this.formatter, text),
                error: this.formatter.error.bind(this.formatter, text),
                rawError: this.formatter.rawError.bind(this.formatter)
            };

        parser.ast.init();
        var error_cnt = this.parse(text);
        if (error_cnt != 0)
            return { errors: error_cnt, warnings: 0 };

        wvmCodes.init(); // Important to set up the byte codes table.
            // The library codes can depend on the base byte codes, so now that
            // the base byte codes are known, the libraries can initialise their
            // byte codes.
        libraries.init();
        new AssemblerAstProcessor(formatter, libraries).process(parser.ast);
        var generator = new WvmGenerator(formatter, libraries);
        return generator.generate(parser.ast);
    };

module.exports = WetaAssembler;

