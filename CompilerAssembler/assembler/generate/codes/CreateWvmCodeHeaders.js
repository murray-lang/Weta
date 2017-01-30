var wvmCodes = require('./WvmCodes')
var libraries    = require('../../../lib/Libraries');
var fs       = require('fs');

// Running this file will output the codes in a C enum format

function listCodes(codes, libName)
{
    var initData      = codes.initData;
    var isBase        = libName == "base";
    var libPrefix     = isBase ? "" : libName.toUpperCase() + "_";
        // For base codes we're interested only in the first code of any
        // compound codes, otherwise we're only interested in the second.
    var codeIndex     = isBase ? 0 : 1;
        // Don't even consider base codes unless they have only one, because
        // compound codes in the base library are all self referential
        // anyway (ie. they are already declared)
    var codeLength = isBase ? 1 : 2;
    var result = "";
    var libCodes = codes.codes;
    for (var i = 0; i < initData.length; i++)
    {
        var name  = initData[i].asm;
        var bytecodes = libCodes[name];
        name = name.replace(new RegExp("\\.", 'g'), "_");

        if (bytecodes.length == codeLength)
        {
            var thisCode = bytecodes[codeIndex];
            result += "\tOP_" + libPrefix + name.toUpperCase() + "\t\t= " + thisCode;
            if (thisCode < 255)
                result += ",";
            result += "\n";
        }
    }
    return result;
}

function camelCase(str)
{
    return str.charAt(0).toUpperCase() + str.slice(1);
}

function createHeader(codes, libName)
{
    var Libname  = camelCase(libName);
    var fileBase = "Wvm" + Libname + "Codes";
    var filePath = "./" + fileBase + ".h";
    var enumName = "e" + fileBase;
    var define   = "__" + fileBase.toUpperCase() + "_H__"

    var text =   "#ifndef __" + define + "\n"
               + "#define " + define +"\n\n"
               + "enum " + enumName
               + "\n{\n";

    text += listCodes(codes, libName);
    text += "};\n\n#endif";

    try
    {
        fs.writeFileSync(filePath, text, { encoding: 'utf8' });
    }
    catch (e)
    {
        console.error("Error writing header file " + filePath + ": " + e.message);
    }
}

wvmCodes.init();

for (var libName in wvmCodes)
{
    createHeader(wvmCodes[libName], libName);
}
libraries.init();

for (var libName in libraries)
{
    createHeader(libraries[libName].codes, libName);
}

