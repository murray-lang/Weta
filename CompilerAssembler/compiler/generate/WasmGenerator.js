var AstTraverser          = require('../../common/AstTraverser');
var Types                 = require('../../common/Types');
ParseConfigs              = require('../../common/traverse/ParseConfigs');
CompileNodeType           = require('../common/AstNodes/NodeType');
VariableNodeKind          = require('../common/AstNodes/VariableNode').VariableNodeKind;
var VarAssignmentNodeKind = require('../common/AstNodes/VarAssignmentNode').VarAssignmentNodeKind;
var ControlNodeKind       = require('../common/AstNodes/ControlNode').ControlNodeKind;
var CommandNodeKind       = require('../common/AstNodes/CommandNode').CommandNodeKind;
var VarFetchNodeKind      = require('../common/AstNodes/VarFetchNode').VarFetchNodeKind;
var ExpressionNodeKind    = require('../common/AstNodes/ExpressionNode').ExpressionNodeKind;
var InputNodeKind         = require('../common/AstNodes/InputNode').InputNodeKind;
var MotorNodeKind         = require('../common/AstNodes/MotorNode').MotorNodeKind;
PrepareforCdecl           = require('./traverse/PrepareforCdecl');
MakeIteratorsPointers     = require('./traverse/MakeIteratorsPointers');
GetAssemblerDefines       = require('./traverse/GetAssemblerDefines');

var typeDirectives =
    {
        "int8":     { directive: ".byte",   initialValue: "0" },
        "uint8":    { directive: ".byte",   initialValue: "0" },
        "int16":    { directive: ".short",  initialValue: "0" },
        "uint16":   { directive: ".short",  initialValue: "0" },
        "int32":    { directive: ".int",    initialValue: "0" },
        "uint32":   { directive: ".int",    initialValue: "0" },
        "float":    { directive: ".single", initialValue: "0.0" },
        "double":   { directive: ".double", initialValue: "0.0" },
        "bool":     { directive: ".byte",   initialValue: "0" },    // Hmmm... TODO: Booleans as bytes?
        "ptr":      { directive: ".ptr",    initialValue: "0" },
        "string":   { directive: ".asciz",   initialValue: "\"\"" }
    };

var ioCodeMap =
    {
        digitalin:  "din",
        digitalout: "dout",
        analogin:   "ain",
        analogout:  "aout"
    };

function sizeOf(type)
{
    var i = 0;
    var size = 1;
    var arg = type[i].code;
//    if (arg == "string")
//    {
//            // Add 1 for the terminating 0.
//        size = type[i].size + 1;
//    }
//    else
//    {
        while (arg == "array")
        {
            arg = type[i + 1].code
            size *= type[i].size;
            i++;
        }
//    }
    var result = "sizeof(" + typeDirectives[arg].directive + ")";
    if (size > 1)
        result += " * " + size;
    return result;
}



function WasmGenerator (globals, procDefs, stringLiterals, libraries, output, formatter)
{
    this.globalVars           = globals;
    this.procedureDefinitions = procDefs;
    this.stringLiterals       = stringLiterals;
    this.libraries            = libraries;
    this.extOutput            = output;
    this.formatter            = formatter;
    this.indent               = "\t";

    this.stringLiteralSymbolBase = "strLiteral";
    this.stringLiteralNext  = 0;

    this.alignment = 2;
    this.cdeclParamsByProc = {};
}
WasmGenerator.prototype = new AstTraverser();
WasmGenerator.prototype.constructor = WasmGenerator;

WasmGenerator.prototype.generate =
    function (nodes)
    {
        this.prepare(nodes);
        this.output(".global _start\n");
        this.emitDefines(nodes);
        this.output(""); // ie. extra line
        this.emitData();
        this.output(".text");
        //this.output(".align " + this.alignment);
        this.output("_start:", false);
        //this.output("begin");
        this.exclusions = { "procedure":  true };
        this.traverse(nodes);
        this.output("exit");
        this.output(""); // ie. extra line
        this.exclusions = undefined;
        this.emitProcedures();
        this.output(".end");
    };

WasmGenerator.prototype.output =
    function (str, doIndent)
    {
        var indent;
        if (doIndent !== undefined && doIndent == false)
            indent = "";
        else
            indent = this.indent;
        this.extOutput(indent + str + "\n");
    };

WasmGenerator.prototype.prepare =
    function (node)
    {
        new PrepareforCdecl().traverse(node, this.cdeclParamsByProc);
        new ParseConfigs(this.formatter, this.libraries).traverse(node);
        new MakeIteratorsPointers(this.formatter).traverse(node, this.globalVars, {});
    };

WasmGenerator.prototype.typeAsDirective =
    function (type, tabs, index, initialValue)
    {
        if (index === undefined)
            index = 0;
        if (tabs === undefined)
            tabs = "";

        var str = "";
        if (index >= 0)
        {
            if (type[index].code == "array")
            {
                str += "\n" + tabs + ".rept " + type[index].size + "\n";
                str += this.typeAsDirective(type, tabs + "\t", index + 1) + "\n";
                str += tabs + ".endr";
            }
            else if (type[index].code == "string")
            {

                var dir = typeDirectives["string"].directive;

                var init = "\"";
                if (initialValue === undefined)
                {
                        // initialise to a string of spaces of the given length
                    for (var i = 0; i < type[index].length; i++)
                        init += " ";
                }
                else
                {
                    init += initialValue;
                }
                init += "\"";
                /*
                    // Add 1 to the size for terminating 0.
                str += "\n" + tabs + ".rept " + (type[index].size + 1) + "\n";
                str += tabs + "\t" + dir + "\t" + init + "\n";
                str += tabs + ".endr";
                */
                str += tabs + dir + "\t" + init;
            }
            else
            {
                if (!(type[index].code in typeDirectives))
                {
                    this.formatter.error(false, null, "A variable cannot be of type %s", type[index].toString());
                    return null;
                }
                var dir = typeDirectives[type[index].code];
                str = tabs + dir.directive + "\t\t";
                if (initialValue === undefined)
                    str += dir.initialValue;
                else
                    str += initialValue;
            }
        }
        return str;
    };

WasmGenerator.prototype.emitStringLiterals =
    function()
    {
        for (var literal in this.stringLiterals)
        {
                // Only emit the literal in the global table if it is used more
                // than once or if it's assigned to a variable.
            if (   this.stringLiterals[literal].count > 1
                || this.stringLiterals[literal].isAssigned)
            {
                var nextSymbol = this.stringLiteralSymbolBase + this.stringLiteralNext++;
                    // Make the symbol available for later traversals
                this.stringLiterals[literal].symbol = nextSymbol;
                this.output(nextSymbol + ":\t\t.asciz \"" + literal + "\"", false);
            }
        }
    };

WasmGenerator.prototype.emitDefines =
    function (nodes)
    {
        var defines = new GetAssemblerDefines(this.formatter).traverse(nodes, this.indent);
        if (defines.length > 0)
            this.output(defines, false);
    };

WasmGenerator.prototype.emitVariable =
    function (name, varInfo, indent)
    {
            // Don't emit variables that have been renamed
            // (they are elsewhere in the table)
        if (varInfo.renamed == null)
        {
            if (Types.isUnknown(varInfo.type))
            {
                this.formatter.error(false, null, "A type has not been determined for variable '%s'", name);
            }
            else
            {
                var dir = this.typeAsDirective(varInfo.type, "\t", 0, varInfo.value);
                if (dir)
                    this.output(name + ":" + dir, indent);
                else
                    this.formatter.error(false, null, "Invalid type for variable '%s'", name);
            }
        }
    };

WasmGenerator.prototype.emitData =
    function ()
    {
        this.output(".data");
        //this.output(".align " + this.alignment);
        //this.emitStringLiterals();
        for (var varName in this.globalVars)
        {
            this.emitVariable(varName, this.globalVars[varName], false);
        }
        this.output(""); // ie. new line
    };

WasmGenerator.prototype.emitProcedures =
    function ()
    {
        for (var procName in this.procedureDefinitions)
        {
            this.emitProcedure(this.procedureDefinitions[procName]);
            this.output(""); // ie new line
        }
    };

WasmGenerator.prototype.emitProcedure =
    function (procNode)
    {
        this.output(procNode.name + ":\t\t.proc", false);
        var numParams = this.emitParameters(procNode);
            // If there are parameters or local variables then we need to add
            // enter and leave instructions to save and restore the procedure's
            // local frame respectively.
        var enterLeave = numParams > 0 || Object.keys(procNode.localVars).length > 0;
        this.output("begin");
        if (enterLeave)
            this.output("enter");   // Save the local frame
        this.emitLocals(procNode);
        this.traverse(procNode.children[1]);
        if (enterLeave)
            this.output("leave");   // restore the local frame
        this.output("return");
        this.output(".endproc");
    };

WasmGenerator.prototype.emitParameters =
    function (procNode)
    {
        // Get the parameter information for the  procedure
        var cdeclParams = this.cdeclParamsByProc[procNode.name];

        if (cdeclParams.asArray.length > 0)
        {

            this.output(".params");
            for (var i = 0; i < cdeclParams.asArray.length; i++)
                this.output(cdeclParams.asArray[i].name + ":" + this.typeAsDirective(cdeclParams.asArray[i].resultType, "\t"), true);
            this.output(".endparams\n");
        }
        return cdeclParams.asArray.length;
    };

WasmGenerator.prototype.emitLocals =
    function (procNode)
    {
        if (Object.keys(procNode.localVars).length == 0)
            return; // No localVars

        this.output(".locals");
        for (var varName in procNode.localVars)
            this.emitVariable(varName, procNode.localVars[varName], true);
        this.output(".endlocals\n");
    };

WasmGenerator.prototype["empty"] =
    function (node)
    {
    };

WasmGenerator.prototype["generic"] =
    function (node)
    {
        this.traverseChildren(node);
    };

WasmGenerator.prototype["config"] =
    function (node)
    {
        this.traverseChildren(node);
        /*
        if (node.item in ioCodeMap)
            this.output("config." + ioCodeMap[node.item]);
        else
            this.output("config." + node.item);
        */
    };

WasmGenerator.prototype["lib"] =
    function (node)
    {
        this.traverseChildren(node);
        this.output(node.token.match);
    };

WasmGenerator.prototype[CompileNodeType.declaration] =
    function (node)
    {
    };

WasmGenerator.prototype["list"] =
    function (node)
    {
            // emit from right to left so that the firmware can pop them off
            // the stack in left to right order.
        for (var i = node.children.length - 1; i >= 0; i--)
        {
            this.traverse(node.children[i]);
            this.castNode(node.children[i], [node.resultType[1]]);
        }

            // emit the number of items in the list
        this.output("uint8\t\t" + node.children.length);
    };

WasmGenerator.prototype[CompileNodeType.variable] =
    function (node)
    {
        this.output(node.scope + "\t\t" + node.name);
        if (node.things == 0 || node.kind == VariableNodeKind.iterator)
            return;

        var noMorePointers = false;
        for (var nextThing = 1; nextThing <= node.things; nextThing++)
        {
            if (noMorePointers)
            {
                this.formatter.error(false, node.token, "Cannot dereference %s any further", node.name);
               return;
            }
                // The ':' before the name of an array results in an additional
                // "thing", but this is inconsistent with other variables. The
                // implied dereference of the array variable is a quirk of Logo
                // and wrong as far as the implementation goes.
            if (node.type[nextThing].code != "array")
            {
                var cmd = node.type[nextThing].prefix + "get";
                this.output(cmd);
            }
            if (node.type[nextThing].code != "ptr")
                noMorePointers = true;
        }
    };

WasmGenerator.prototype[CompileNodeType.assignment] =
    function (node)
    {
            // Cricket Logo expects the receiving variable address to be on the
            // stack before the data being assigned. This makes reading the
            // assembly language difficult because the variable address and the
            // corresponding "set" command can be separated by a lot of
            // codes. For now I am going to break compatibility with that by
            // commenting out the code below and placing it just before the set.
        // this.traverse(node.variable);

            // If this is the initial assignment and it was with an immediate
            // value then the assignment will be performed as part of the
            // data initialisation. There is no need to perform the assignment
            // again here.
        if (node.isInitial)
            return;

        var resultType  = node.variable.resultType;
            // make takes the name of (pointer to) the variable, so dereference
            // to determine the data type
        if (node.kind == VarAssignmentNodeKind.make)
            resultType = Types.fromPointer(resultType);

            // emit the value codes
        if (node.kind == VarAssignmentNodeKind.make)
            this.castChildren(node, true, resultType);
        else if (node.kind == VarAssignmentNodeKind.aset)
            this.castChildren(node, true, resultType, [Types.int16]);

        // Now back to the variable setting
        this.traverse(node.variable);
        var cmd = Types.prefixes(resultType) + "set";

        this.output(cmd);
    };

WasmGenerator.prototype[CompileNodeType.expression] =
    function (node)
    {
            // The expression node will already have had its type harmonised
            // with its children during the AST fixup. Now the child
            // outputs can be cast to the type of the resulting expression.
            // Presumably at least one of the children's output need not
            // be cast.
        if (node.kind == ExpressionNodeKind.logic)
            this.logicExpression(node);
        else if (node.kind == ExpressionNodeKind.math)
            this.mathExpression(node);
        else if (node.kind == ExpressionNodeKind.convert)
            this.convertExpression(node);
    };

WasmGenerator.prototype.convertExpression =
    function (node)
    {
        switch (node.operator)
        {
        case "tostring":
            {
                this.traverseChildren(node);
                var prefix = Types.prefixes(node.children[0].resultType);
                this.output(prefix + "tostr");
            }
            break;

        case "ascii":
            this.castChildren(node, false, [Types.string(0)]);
            this.output("ascii");
            break;

        case "count":
            this.castChildren(node, false, [Types.string(0)]);
            this.output("strlen");
            break;
        }
    };

WasmGenerator.prototype.logicExpression =
    function (node)
    {
        var commonType;
        var usePrefix = true;
        switch (node.operator)
        {
        case "eq":
        case "lt":
        case "gt":
        case "le":
        case "ge":
        case "ne":
            // The expression resultType is boolean, but the operands can
            // be anything as long as they are both the same. Harmonise.
            commonType = Types.harmonise(node.children[0].resultType, node.children[1].resultType);
            break;

        case "and":
        case "or":
        case "xor":
            // Both children must be boolean.
            commonType = [Types.bool];
            if (node.children[0].resultType[0] != Types.bool
                || node.children[1].resultType[0] != Types.bool)
            {
                this.formatter.error(false, node.token, "Both operands of %s must be boolean", node.token.token);
                return;
            }
            usePrefix = false;  // No prefix required as boolean is implicit
            break;

        case "not":
            // Child must be boolean.
            commonType = [Types.bool];
            if (node.children[0].resultType[0] != Types.bool)
            {
                this.formatter.error(node.token, "Operand of %s must be boolean", node.token.token);
                return;
            }
            usePrefix = false; // No prefix required as boolean is implicit
            break;
        }
        this.castChildren(node, false, commonType);
        var prefix = usePrefix ? commonType[0].prefix : "";
        this.output(prefix + node.operator);
    };

WasmGenerator.prototype.mathExpression =
    function (node)
    {
        var library = "";
        if (   node.operator == "ashift"
            || node.operator == "lshift"
            || node.operator == "rotate")
        {
            this.castNode(node.children[0], node.resultType);
            this.castNode(node.children[1], [Types.int8]);
            this.output(node.resultType[0].prefix + node.operator);
        }
        else
        {
            this.castChildren(node, false, node.resultType);
            switch (node.operator)
            {
            case "pow":
            case "sqr":
            case "sqrt":
            case "exp":
            case "sin":
            case "cos":
            case "tan":
            case "asin":
            case "acos":
            case "atan":
            case "atan2":
            case "sinh":
            case "cosh":
            case "tanh":
            case "hypot":
            case "ln":
            case "log10":
            case "floor":
            case "ceil":
            case "rnd":
            case "trunc":
            case "isnan":
            case "isinf":
                library = "math.";
            }
            this.output(library + node.resultType[0].prefix + node.operator);
        }
    };

WasmGenerator.prototype[CompileNodeType.fetch] =
    function (node)
    {
        this.traverseChildren(node);
        this.traverse(node.variable);

        if (node.kind == VarFetchNodeKind.aget)
        {
            var resultType  = node.variable.resultType;
            var code = Types.prefixes(resultType) + "get";
            this.output(code);
        }
    };

WasmGenerator.prototype[CompileNodeType.call] =
    function (node)
    {
        if (!(node.name in this.procedureDefinitions))
        {
            this.formatter.error(false, node.token, "Call to undefined procedure %s", node.name);
            return;
        }
        var procDef = this.procedureDefinitions[node.name];
            // If the procedure returns a value then reserve a place on the
            // stack for that result to be put. It needs to be pushed
            // before the arguments because they will be popped of the stack
            // before the result can be used
        var returnType = procDef.resultType[0];
        var checkPointAdded = false;
        if (returnType != Types.void)
        {
                // If the return value is not going to be consumed, then we need
                // to put a checkpoint before the return value placeholder so
                // that it gets cleaned up too.
            if (!node.returnValueExpected)
            {
                this.output("chkpoint");
                checkPointAdded = true;
            }
            this.output(procDef.resultType[0].code + "\t\t" + typeDirectives[procDef.resultType[0].code].initialValue);
        }
            // Output the arguments to this call
        if (node.argsNode === undefined)
        {
                // If the procedure expects arguments then a size must be
                // supplied, even if it is 0. (Procedures with no parameters
                // will not look for arguments so the size need not be
                // provided)
            if (procDef.numParameters > 0)
            {
                    // If there is a return value placeholder, and it's not a
                    // string then there will already be a 0 on the stack, so
                    // a separate args size need not be provided. Otherwise
                    // push it.
                if (returnType == Types.void || returnType.code == "string")
                {
                    if (!checkPointAdded)
                    {
                        // Add a checkpoint before the arg size so that it gets
                        // cleaned up by a rollback
                        this.output("chkpoint");
                        checkPointAdded = true;
                    }
                    this.output("span\t\t0");
                }
            }
        }
        else
        {
                // If a checkpoint isn't already added then add it now so that
                // the arguments get cleaned up by a rollback
            if (!checkPointAdded)
            {
                this.output("chkpoint");
                checkPointAdded = true;
            }
                // Get the parameter information for the called procedure
                // (These ones are in the correct order)
            var cdeclParams = this.cdeclParamsByProc[node.name];
            this.traverse(node.argsNode, cdeclParams, returnType.code == "string");
        }
        this.output("cptr\t\t" + node.name);
        this.output("call");
              // Cleanup code here
        if (checkPointAdded)
            this.output("rollback");
    };

WasmGenerator.prototype[CompileNodeType.block] =
    function (node)
    {
        this.output("block");
        this.traverseChildren(node);
        this.output("eob");
    };

WasmGenerator.prototype[CompileNodeType.command] =
    function (node)
    {
        var cmd    = node.kind.toString();
        var suffix = "";
        var prefix = "";
        switch (node.kind)
        {
        case CommandNodeKind.setpower:
            this.traverse(node.children[0]);
            this.castNode(node.children[0], [Types.uint8]);
            break;
        case CommandNodeKind.setsvh:
        case CommandNodeKind.svr:
        case CommandNodeKind.svl:

        case CommandNodeKind.setdp:
        case CommandNodeKind.record:
        case CommandNodeKind.erase:
        case CommandNodeKind.onfor:
            this.traverse(node.children[0]);
            this.castNode(node.children[0], [Types.int16]);
            break;

        case CommandNodeKind.send:
            {

                if (node.children.length == 1)
                {
                        // First child (value) should not be cast because the
                        // prefix will be set to deal with the data type.
                    this.traverse(node.children[0]);
                        // The only child is the value to send to the
                        // default port. This version of send requires
                        // a prefix to specify the 'with' type.
                    var type = node.children[0].resultType;
                    prefix = Types.prefixes(type);
                    cmd = "tx";
                }
                else if (node.children.length == 3)
                {
                        // Number of bytes to send (or length of array)
                    this.traverse(node.children[2]);
                    this.castNode(node.children[2], [Types.uint8]);
                        // Buffer is not cast
                    this.traverse(node.children[1]);
                        // Port number
                    this.traverse(node.children[0]);
                    this.castNode(node.children[0], [Types.uint8]);

                        // No prefix for this version of send. Just add the
                        // "n" suffix to specify the array version
                    this.output("txn");
                        // This version of send always returns the number of
                        // bytes sent as a uint8. However, the caller might or
                        // might not be interested in it. If they are not
                        // interested then throw the value away.
                    if (!node.returnValueExpected)
                    {
                        this.output("uint8\t1");
                        this.output("pop");
                    }
                    return;
                }

            }
            break;

        case CommandNodeKind.received:
            cmd = "newrx"
            if (node.children.length == 1)
            {
                this.traverse(node.children[0]);
                this.castNode(node.children[0], [Types.uint8]);
                cmd += "n";
            }
            suffix = "?";
            break;

        case CommandNodeKind.receive:
            {

                if (node.children.length == 0)
                {
                        // No argument, but we need to prefix the command for
                        // the type of data required
                    var type = node.resultType;
                    prefix = Types.prefixes(type);
                    cmd = "rx";
                }
                else if (node.children.length == 4)
                {
                        // timeout
                    this.traverse(node.children[3]);
                    this.castNode(node.children[3], [Types.int16]);
                        // Number of bytes to receive
                    this.traverse(node.children[2]);
                    this.castNode(node.children[2], [Types.uint8]);
                        // Buffer is not cast
                    this.traverse(node.children[1]);
                        // Port number
                    this.traverse(node.children[0]);
                    this.castNode(node.children[0], [Types.uint8]);

                        // No prefix for this version of receive. Just add the
                        // "n" suffix to specify the array version
                    this.output("rxn");
                    // This version of receive always returns the number of
                    // bytes received as a uint8. However, the caller might or
                    // might not be interested in it. If they are not
                    // interested then throw the value away.
                    if (!node.returnValueExpected)
                    {
                        this.output("uint8\t1");
                        this.output("pop");
                    }
                    return;
                }

            }
            break;

        case CommandNodeKind.digitalout:
                // The parser grammar only allows a boolean expression for the
                // second parameter, so no casting should be required. It goes
                // on the VM stack first
            this.traverse(node.children[1]);
            this.traverse(node.children[0]); // Output number
            this.castNode(node.children[0], [Types.uint8]);
            cmd = ioCodeMap[cmd];
            break;

        case CommandNodeKind.analogout:
            this.traverse(node.children[1]);
            this.castNode(node.children[1], [Types.uint8]);

            this.traverse(node.children[0]);
            this.castNode(node.children[0], [Types.uint8]);
            cmd = ioCodeMap[cmd];
            break;

        case CommandNodeKind.i2c:
            if (node.command == "i2cread" || node.command == "i2cwrite")
            {
                this.traverse(node.children[4]);                 // Buffer length
                this.castNode(node.children[4], [Types.uint8]);  // Cast buffer length
                this.traverse(node.children[3]);                 // Buffer variable


                this.traverse(node.children[1]);                 // Register address
                    // The register address width is emitted after the register
                    // address so that it goes onto the stack later. This means
                    // that it can be popped first and used to determine what
                    // data type needs to be popped for the register address.
                this.traverse(node.children[2]);                 // Register address width
                this.castNode(node.children[2], [Types.uint8]);  // Cast register address width
                this.traverse(node.children[0]);                 // I2C address
                this.castNode(node.children[0], [Types.uint8]); // Cast I2C address
            }
            this.output(node.command);
            return;

        case CommandNodeKind.encode:
                // Length first...
            this.traverse(node.children[1]);
            this.castNode(node.children[1], [Types.uint8]);  // Length limited to 255
                //...then the array.
            this.traverse(node.children[0]);
                // Command variant is determined by the input array type
            prefix = Types.endType(node.inputType).prefix;
            break;

        case CommandNodeKind.decode:
                // No need to traverse the first child because it is just the
                // output type specification that has already been used to
                // determine the result type
                // Length first...
            this.traverse(node.children[2]);
            this.castNode(node.children[2], [Types.uint8]);  // Length limited to 255
                //...then the array.
            this.traverse(node.children[1]);

                // Command variant is determined by the output list type
            prefix = Types.endType(node.resultType).prefix;
            break;

        case CommandNodeKind.math:
            this.castChildren(node, false, [Types.float]);
            this.output(node.command);
            return;

        case CommandNodeKind.forward:
        case CommandNodeKind.backward:
        case CommandNodeKind.left:
        case CommandNodeKind.right:
            this.traverse(node.children[0]);
            this.castNode(node.children[0], [Types.int16]);
            break;
        }
        this.output(prefix + cmd + suffix);
    };

WasmGenerator.prototype[CompileNodeType.control] =
    function (node)
    {
        switch (node.kind)
        {
        case ControlNodeKind.dowhile:
            this.output("do");
            // Fall through
        case ControlNodeKind.while:
            this.traverse(node.children[1]);    // Block first...
            this.traverse(node.children[0]);    // ... then the condition
            this.output("while");
            break;

        case ControlNodeKind.output:
            this.castChildren(node, false, node.resultType);
            this.output(Types.prefixes(node.resultType) + "output");
                // Append a return after each "output"
                // TODO: be smarter about this
            this.output("return");
            break;

        case ControlNodeKind.foreach:
            this.traverse(node.children[1]);    // List first
            this.traverse(node.children[0]);    // then iterator
            this.traverse(node.children[2]);    // then block
            this.output(Types.prefixes(node.children[0].resultType)+ node.kind.toString());
            break;


            this.traverseChildren(node);
                // Wait is specified by
            break;

        case ControlNodeKind.waitms:
        case ControlNodeKind.wait:
            this.traverseChildren(node);
            this.output(ControlNodeKind.wait.toString());
            break;
        case ControlNodeKind.if:
        case ControlNodeKind.ifelse:
        case ControlNodeKind.repeat:
        case ControlNodeKind.forever:
        case ControlNodeKind.waituntil:
        case ControlNodeKind.tag:
        case ControlNodeKind.goto:
        case ControlNodeKind.for:
            this.traverseChildren(node);
            // Fall through
        case ControlNodeKind.return: // No arguments.
            this.output(node.kind.toString());
            break;
        }
    };

WasmGenerator.prototype[CompileNodeType.input] =
    function (node)
    {
        var outputChildren = true;
        var childType = [Types.int16];
        var str = node.kind.toString();
        switch (node.kind)
        {
        case InputNodeKind.repcount:
            str = "slot";
            //Fall through
        case InputNodeKind.slot:
            str = Types.prefixes(node.resultType) + str;
            break;

        case InputNodeKind.sensor:
        case InputNodeKind.switch:
                // for switches and sensors the child is just a digit to append
                // to the mnemonic.
            str += node.children[0].value;
            outputChildren = false;
            break;

        case InputNodeKind.random:
            if (node.children.length > 0)
                str += "xy";
            break;

        case InputNodeKind.digitalin:
        case InputNodeKind.analogin:
            childType = [Types.int8];
            str = ioCodeMap[str];
            break;
        case InputNodeKind.timer:
        case InputNodeKind.recall:

        }
        if (outputChildren)
            this.castChildren(node, false, childType);
        this.output(str);
    };

WasmGenerator.prototype[CompileNodeType.immediate] =
    function (node, isAssignment)
    {
            // If this is a string literal then try to find it in the global
            // table. If it's there then emit code to retrieve it from the
            // table rather than emit the literal directly.

        if (node.resultType[0].code == "string")
        {
            this.output(node.resultType[0].code + "\t\t\"" + node.value + "\"");
            /*
            if (node.value in this.stringLiterals)
            {
                if (this.stringLiterals[node.value].symbol !== undefined)
                {
                    this.output("global\t\t" + this.stringLiterals[node.value].symbol);
                        // If this string is being assigned to a variable then the
                        // variable will be a pointer, and the above will be enough.
                        // Otherwise we need to get the actual string onto the stack.
                    if (isAssignment !== undefined && isAssignment == true)
                    {
                            // The result type has become a pointer as a consequence
                            // of the above. Reflect it in the type so that casting
                            // isn't messed up.
                        node.resultType.unshift(Types.pointer);
                    }
                    else
                    {
                        this.output("strget");
                    }
                    return;
                }
            }
            */
        }
        else
        {
            this.output(node.resultType[0].code + "\t\t" + node.value);
        }
    };

WasmGenerator.prototype[CompileNodeType.procedure] =
    function (node)
    {
        // Do nothing with procedure nodes. They will be processed within their
        // procedure definitions.
    };

WasmGenerator.prototype[CompileNodeType.motor] =
    function (node)
    {
        if (node.kind == MotorNodeKind.servo)
            this.selectServos(node);
        else
            this.selectMotors(node);
    };

WasmGenerator.prototype[CompileNodeType.arguments] =
    function (node, cdeclParams, forceArgSize, cleanup, extraPopType)
    {
        if (cdeclParams.asArray.length != node.children.length)
        {
            this.formatter.error(false, node.token,
                "%s requires %n parameters but has been supplied with %n",
                node.token.value, cdeclParams.asArray.length,
                node.children.length
            );
            return;
        }
        var totalSize;
        // Pop arguments
        for (var i = 0; i < node.children.length; i++)
        {
                // Accumulate size information
            var nextSize = sizeOf(cdeclParams.asArray[i].resultType);
            if (totalSize === undefined)
                totalSize = "span\t\t" + nextSize;
            else
                totalSize += " + " + nextSize;
        }

        if (cleanup === undefined)
        {
                // Call arguments are pushed in right to left
                // (cdecl-like convention)
            for (var i = node.children.length - 1; i >= 0; i--)
            {
                this.traverse(node.children[i]);
                    // See if a type conversion is required
                this.castNode(node.children[i], cdeclParams.asArray[i].resultType);
            }
                // Now the total size of the arguments is pushed. This allows
                // the called procedure to check if all of the parameters
                // have been supplied, and also to calculate where to put the
                // return value.
            if (totalSize !== undefined)
                this.output(totalSize);
            else if (forceArgSize)
                this.output("span\t\t0");
        }
        else
        {
                // When cleaning up, we also need to dispose of the size value
                // that was pushed onto the stack above.
            if (totalSize !== undefined)
                totalSize += " + " + sizeOf([Types.uint8]);
            else if (forceArgSize)
                totalSize = sizeOf([Types.uint8])

                // If there's a return value to be disposed of then add it
            if (extraPopType !== undefined)
            {
                if (totalSize === undefined)
                    totalSize = "span\t\t";
                else
                    totalSize += " + ";

                totalSize += sizeOf(extraPopType);
            }
            if (totalSize !== undefined)
            {
                this.output(totalSize);
                this.output("pop");
            }
        }
    };

WasmGenerator.prototype.getConversion =
    function (typeFrom, typeTo)
    {
        return typeFrom.prefix + "to" + typeTo.prefix;
    };

WasmGenerator.prototype.castNode =
    function (node, resultType)
    {
            // If both types are pointers then no casting is required,
            // regardless of what they point to
        if (node.resultType[0] == Types.pointer && resultType[0] == Types.pointer)
            return;

            // If they are both strings then do nothing
        if (node.resultType[0].code == "string" && resultType[0].code == "string")
            return;

            // If they are just signed or unsigned versions of each other then
            // don't cast because the bits won't change. It's all in the
            // interpretetion.
        if (node.resultType[0].size == resultType[0].size)
            if (node.resultType[0].isSigned !== undefined && resultType[0].isSigned !== undefined)
                return;
        var type;
        // If the result type is an array, then get the type of the
        // array items.
        if (resultType[0].code == "array")
            type = resultType.slice(1, resultType.length);
        else
            type = resultType;

        if (!Types.equals(node.resultType, type))
        {

            this.output(this.getConversion(node.resultType[0], type[0]));
        }
    };

WasmGenerator.prototype.castChildren =
    function (node, isAssignment, resultType)
    {
        var nextArg = 2;
        if (node.children !== undefined)
        {
            for (var i = 0; i < node.children.length; i++)
            {
                this.traverse(node.children[i], isAssignment);
                this.castNode(node.children[i], arguments[nextArg]);
                if (nextArg < arguments.length - 1)
                    nextArg++;
            }
        }
    };

WasmGenerator.prototype.legacySelectMotors =
    function (node)
    {
        var motorTokens =
                [
                    ["a",     1],
                    ["b",     2],
                    ["ab",    3],
                    ["c",     4],
                    ["ac",    5],
                    ["bc",    6],
                    ["abc",   7],
                    ["d",     8],
                    ["ad",    9],
                    ["bd",   10],
                    ["abd",  11],
                    ["cd",   12],
                    ["acd",  13],
                    ["bcd",  14],
                    ["abcd", 15]
                ];
        for (var i = 0; i < motorTokens.length; i++)
        {
            if (node.motors == motorTokens[i][0])
            {
                this.output("uint8\t\t" + motorTokens[i][1]);
                this.output("motors");
                return;
            }
        }
        this.formatter.error(false, node.token, "Motor selection %s not supported", node.motors);
    };

WasmGenerator.prototype.selectMotors =
    function (node)
    {
        // This way of assigning motors to bits is not compatible with the
        // original Babuino, which assigned a token to each possible
        // combination.
        //Elements correspond to motors a, b, c, d, e, f, g, h
        var flags = [0, 0, 0, 0, 0, 0, 0, 0];
        var a = "a".charCodeAt(0);	// Get code for 'a'

        for (var i = 0; i < node.motors.length; i++)
        {
            var c = node.motors.charCodeAt(i);
            var m = c - a;		// Convert to index ('a' == 0)
            flags[m] = 1;			// Flag this motor
        }
        // Now create a binary number as bitwise flags for motors.
        // 'a' is first in the array but will be the LSB, so step
        // backwards through the array to build the argument.
        var	arg = "0b";
        for (var i = 7; i >= 0; i--)
        {
            if (flags[i] == 0)
                arg += '0';
            else
                arg += '1';
        }
        this.output("uint8\t\t" + arg);
        this.output("motors");
    };

WasmGenerator.prototype.selectServos =
    function (node)
    {
        //Elements correspond to motors n, o, p, q, r, s, t, u
        var flags = [0, 0, 0, 0, 0, 0, 0, 0];
        var n = "n".charCodeAt(0);	// Get code for 'n'

        for (var i = 0; i < node.motors.length; i++)
        {
            var c = node.motors.charCodeAt(i);
            var x = c - n;		// Convert to index ('n' == 0)
            flags[x] = 1;			// Flag this servo
        }
        // Now create a binary number as bitwise flags for servos.
        // 'n' is first in the array but will be the LSB, so step
        // backwards through the array to build the argument.
        var	arg = "0b";
        for (var i = 7; i >= 0; i--)
        {
            if (flags[i] == 0)
                arg += '0';
            else
                arg += '1';
        }
        this.output("uint8\t\t" + arg);
        this.output("servos");
    };

module.exports = WasmGenerator;
