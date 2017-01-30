/**
 * This file provides a console-based wrapper around
 * @module WetaLogoMain
 * @type {WetaLogo|exports}
 * @requires WetaLogo, MessageFormatter
 *
 */
WetaLogo       = require('./WetaLogo')
var fs            = require('fs');
MessageFormatter  = require('../../common/MessageFormatter');

var wasmText = "";

function outputHandler(str)
{
    wasmText += str;
}

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

if( process.argv.length > 2 )
{
    var logo;
    var logoFileName = process.argv[2];
    var wasmFileName = process.argv[3];

    try
    {
        logo = fs.readFileSync( logoFileName, { encoding: 'utf8' });
    }
    catch (e)
    {
        formatter.rawError("Error reading file %s: %s", logoFileName, e.message);
        process.exit(1);
    }
    formatter.rawInfo("Compiling file %s", logoFileName);
    var bl = new WetaLogo(outputHandler, formatter);
    try
    {
        bl.compile(logo);
    }
    catch(e)
    {
        formatter.rawError("Error compiling file %s: %s", logoFileName, e.message);
        process.exit(1);
    }
    formatter.rawInfo("Writing wasm to file %s", wasmFileName);
    try
    {
        fs.writeFileSync(wasmFileName, wasmText, { encoding: 'utf8' });
    }
    catch (e)
    {
        formatter.rawError("Error writing wasm file %s: %s", wasmFileName, e.message);
        process.exit(1);
    }
    process.exit(0);
}
else
{
    formatter.rawError("usage: WetaLogo.js <logo file in> <wasm file out>");
    process.exit(1);
}