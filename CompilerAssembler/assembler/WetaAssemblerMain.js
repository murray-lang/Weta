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

WetaAssembler = require('./WetaAssembler');
var fs           = require('fs');
MessageFormatter = require('../common/MessageFormatter');


function debugHandler(str)
{
    console.log(str);
}

function infoHandler(str)
{
    console.log(str);
}

function warningHandler(str)
{
    console.warn(str);
}

function errorHandler(str)
{
    console.error(str);
}

// This code will be called when the generated script is run
var formatter = new MessageFormatter('./i18n', 'es', debugHandler, infoHandler, warningHandler, errorHandler);
    // The assembler AST requires a formatter to output messages
//parser.ast.setFormatter(formatter);

if( process.argv.length > 2 )
{
    var wasm;
    var wasmFileName = process.argv[2];
    var outFileName = process.argv[3];
    try
    {

        wasm = fs.readFileSync( wasmFileName, { encoding: 'utf8' });
    }
    catch (e)
    {
        formatter.rawError("Error reading file %s: %s", wasmFileName, e.message);
        process.exit(1);
    }
    formatter.rawInfo("Assembling file %s", wasmFileName);
    var ba = new WetaAssembler(formatter);
    var result = {};
    try
    {
        result = ba.assemble(wasm);
    }
    catch(e)
    {
        formatter.rawError("Error assembling file %s: %s", wasmFileName, e.message);
        process.exit(1);
    }
    if (result.errors)
    {
        formatter.rawError("There were %n errors assembling file %s", result.errors, wasmFileName);
        process.exit(1);
    }
    formatter.rawInfo("Writing byte codes to file %s", outFileName);
    try
    {
        var strJson = JSON.stringify(result);
        fs.writeFileSync(outFileName, strJson, { encoding: 'utf8' });
    }
    catch (e)
    {
        formatter.rawError("Error writing output file %s: %s", outFileName, e.message);
        process.exit(1);
    }
    process.exit(0);
}
else
{
    formatter.rawError("usage: WetaAssembler.js <wasm file in> <byte code file out>" );
    process.exit(1);
}

