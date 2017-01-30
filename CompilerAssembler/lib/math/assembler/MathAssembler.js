

//--------------------------------------------------------------------------
// My stuff

AST               = require('../../../common/Ast');
InstructionNode   = require('../../../assembler/AstNodes/InstructionNode');

var _ast = new AST.AbstractSyntaxTree();
var lib = "math.";


/*
	Default template driver for JS/CC generated parsers running as
	browser-based JavaScript/ECMAScript applications.
	
	WARNING: 	This parser template will not run as console and has lesser
				features for debugging than the console derivates for the
				various JavaScript platforms.
	
	Features:
	- Parser trace messages
	- Integrated panic-mode error recovery
	
	Written 2007, 2008 by Jan Max Meyer, J.M.K S.F. Software Technologies
	
	This is in the public domain.
*/

var MathAsmCC_dbg_withtrace		= false;
var MathAsmCC_dbg_string			= new String();

function alert (text)
{
	console.log(text);
}

function __MathAsmCCdbg_print( text )
{
	MathAsmCC_dbg_string += text + "\n";
}

function __MathAsmCClex( info )
{
	var state		= 0;
	var match		= -1;
	var match_pos	= 0;
	var start		= 0;
	var pos			= info.offset + 1;

	do
	{
		pos--;
		state = 0;
		match = -2;
		start = pos;

		if( info.src.length <= start )
			return 30;

		do
		{

switch( state )
{
	case 0:
		if( info.src.charCodeAt( pos ) == 9 || info.src.charCodeAt( pos ) == 13 || info.src.charCodeAt( pos ) == 32 ) state = 1;
		else if( info.src.charCodeAt( pos ) == 10 ) state = 2;
		else if( info.src.charCodeAt( pos ) == 59 ) state = 27;
		else if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 28;
		else if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 29;
		else if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 30;
		else if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 31;
		else if( info.src.charCodeAt( pos ) == 72 || info.src.charCodeAt( pos ) == 104 ) state = 32;
		else if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 33;
		else if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 34;
		else if( info.src.charCodeAt( pos ) == 77 || info.src.charCodeAt( pos ) == 109 ) state = 35;
		else if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 36;
		else if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 37;
		else if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 38;
		else if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 39;
		else if( info.src.charCodeAt( pos ) == 70 || info.src.charCodeAt( pos ) == 102 ) state = 77;
		else state = -1;
		break;

	case 1:
		state = -1;
		match = 1;
		match_pos = pos;
		break;

	case 2:
		state = -1;
		match = 2;
		match_pos = pos;
		break;

	case 3:
		state = -1;
		match = 19;
		match_pos = pos;
		break;

	case 4:
		if( info.src.charCodeAt( pos ) == 72 || info.src.charCodeAt( pos ) == 104 ) state = 15;
		else state = -1;
		match = 9;
		match_pos = pos;
		break;

	case 5:
		state = -1;
		match = 7;
		match_pos = pos;
		break;

	case 6:
		state = -1;
		match = 4;
		match_pos = pos;
		break;

	case 7:
		state = -1;
		match = 21;
		match_pos = pos;
		break;

	case 8:
		if( info.src.charCodeAt( pos ) == 72 || info.src.charCodeAt( pos ) == 104 ) state = 17;
		else state = -1;
		match = 8;
		match_pos = pos;
		break;

	case 9:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 18;
		else state = -1;
		match = 5;
		match_pos = pos;
		break;

	case 10:
		if( info.src.charCodeAt( pos ) == 72 || info.src.charCodeAt( pos ) == 104 ) state = 19;
		else state = -1;
		match = 10;
		match_pos = pos;
		break;

	case 11:
		state = -1;
		match = 12;
		match_pos = pos;
		break;

	case 12:
		state = -1;
		match = 11;
		match_pos = pos;
		break;

	case 13:
		if( info.src.charCodeAt( pos ) == 50 ) state = 20;
		else state = -1;
		match = 13;
		match_pos = pos;
		break;

	case 14:
		state = -1;
		match = 24;
		match_pos = pos;
		break;

	case 15:
		state = -1;
		match = 16;
		match_pos = pos;
		break;

	case 16:
		state = -1;
		match = 3;
		match_pos = pos;
		break;

	case 17:
		state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 18:
		state = -1;
		match = 6;
		match_pos = pos;
		break;

	case 19:
		state = -1;
		match = 17;
		match_pos = pos;
		break;

	case 20:
		state = -1;
		match = 14;
		match_pos = pos;
		break;

	case 21:
		state = -1;
		match = 23;
		match_pos = pos;
		break;

	case 22:
		state = -1;
		match = 18;
		match_pos = pos;
		break;

	case 23:
		state = -1;
		match = 26;
		match_pos = pos;
		break;

	case 24:
		state = -1;
		match = 25;
		match_pos = pos;
		break;

	case 25:
		state = -1;
		match = 20;
		match_pos = pos;
		break;

	case 26:
		state = -1;
		match = 22;
		match_pos = pos;
		break;

	case 27:
		if( info.src.charCodeAt( pos ) == 10 ) state = 2;
		else if( ( info.src.charCodeAt( pos ) >= 0 && info.src.charCodeAt( pos ) <= 9 ) || ( info.src.charCodeAt( pos ) >= 11 && info.src.charCodeAt( pos ) <= 254 ) ) state = 27;
		else state = -1;
		break;

	case 28:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 40;
		else if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 71;
		else if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 72;
		else state = -1;
		break;

	case 29:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 41;
		else if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 73;
		else state = -1;
		break;

	case 30:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 28;
		else if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 29;
		else if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 31;
		else if( info.src.charCodeAt( pos ) == 72 || info.src.charCodeAt( pos ) == 104 ) state = 32;
		else if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 33;
		else if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 34;
		else if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 36;
		else if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 37;
		else if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 38;
		else if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 39;
		else if( info.src.charCodeAt( pos ) == 70 || info.src.charCodeAt( pos ) == 102 ) state = 42;
		else state = -1;
		break;

	case 31:
		if( info.src.charCodeAt( pos ) == 88 || info.src.charCodeAt( pos ) == 120 ) state = 43;
		else state = -1;
		break;

	case 32:
		if( info.src.charCodeAt( pos ) == 89 || info.src.charCodeAt( pos ) == 121 ) state = 44;
		else state = -1;
		break;

	case 33:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 45;
		else state = -1;
		break;

	case 34:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 3;
		else if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 46;
		else state = -1;
		break;

	case 35:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 47;
		else state = -1;
		break;

	case 36:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 48;
		else state = -1;
		break;

	case 37:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 49;
		else state = -1;
		break;

	case 38:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 50;
		else if( info.src.charCodeAt( pos ) == 81 || info.src.charCodeAt( pos ) == 113 ) state = 51;
		else state = -1;
		break;

	case 39:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 52;
		else if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 53;
		else state = -1;
		break;

	case 40:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 55;
		else state = -1;
		break;

	case 41:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 4;
		else state = -1;
		break;

	case 42:
		if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 58;
		else state = -1;
		break;

	case 43:
		if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 5;
		else state = -1;
		break;

	case 44:
		if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 74;
		else state = -1;
		break;

	case 45:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 60;
		else if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 61;
		else state = -1;
		break;

	case 46:
		if( info.src.charCodeAt( pos ) == 71 || info.src.charCodeAt( pos ) == 103 ) state = 62;
		else state = -1;
		break;

	case 47:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 63;
		else state = -1;
		break;

	case 48:
		if( info.src.charCodeAt( pos ) == 87 || info.src.charCodeAt( pos ) == 119 ) state = 6;
		else state = -1;
		break;

	case 49:
		if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 7;
		else state = -1;
		break;

	case 50:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 8;
		else state = -1;
		break;

	case 51:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 9;
		else state = -1;
		break;

	case 52:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 10;
		else state = -1;
		break;

	case 53:
		if( info.src.charCodeAt( pos ) == 85 || info.src.charCodeAt( pos ) == 117 ) state = 75;
		else state = -1;
		break;

	case 54:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 11;
		else state = -1;
		break;

	case 55:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 12;
		else state = -1;
		break;

	case 56:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 13;
		else state = -1;
		break;

	case 57:
		if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 14;
		else state = -1;
		break;

	case 58:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 76;
		else state = -1;
		break;

	case 59:
		if( info.src.charCodeAt( pos ) == 71 || info.src.charCodeAt( pos ) == 103 ) state = 62;
		else if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 64;
		else state = -1;
		break;

	case 60:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 66;
		else state = -1;
		break;

	case 61:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 67;
		else state = -1;
		break;

	case 62:
		if( info.src.charCodeAt( pos ) == 49 ) state = 68;
		else state = -1;
		break;

	case 63:
		if( info.src.charCodeAt( pos ) == 72 || info.src.charCodeAt( pos ) == 104 ) state = 16;
		else state = -1;
		break;

	case 64:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 21;
		else state = -1;
		break;

	case 65:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 22;
		else state = -1;
		break;

	case 66:
		if( info.src.charCodeAt( pos ) == 70 || info.src.charCodeAt( pos ) == 102 ) state = 23;
		else state = -1;
		break;

	case 67:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 24;
		else state = -1;
		break;

	case 68:
		if( info.src.charCodeAt( pos ) == 48 ) state = 25;
		else state = -1;
		break;

	case 69:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 26;
		else state = -1;
		break;

	case 70:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 3;
		else if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 59;
		else state = -1;
		break;

	case 71:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 56;
		else state = -1;
		break;

	case 72:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 54;
		else state = -1;
		break;

	case 73:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 57;
		else state = -1;
		break;

	case 74:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 65;
		else state = -1;
		break;

	case 75:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 69;
		else state = -1;
		break;

	case 76:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 64;
		else state = -1;
		break;

	case 77:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 28;
		else if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 29;
		else if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 31;
		else if( info.src.charCodeAt( pos ) == 72 || info.src.charCodeAt( pos ) == 104 ) state = 32;
		else if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 33;
		else if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 36;
		else if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 37;
		else if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 38;
		else if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 39;
		else if( info.src.charCodeAt( pos ) == 70 || info.src.charCodeAt( pos ) == 102 ) state = 42;
		else if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 70;
		else state = -1;
		break;

}


			pos++;

		}
		while( state > -1 );

	}
	while( 1 > -1 && match == 1 );

	if( match > -1 )
	{
		info.att = info.src.substr( start, match_pos - start );
		info.offset = match_pos;
		
switch( match )
{
	case 3:
		{
		 info.att = { value: info.att,       token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 4:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 5:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 6:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 7:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 8:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 9:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 10:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 11:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 12:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 13:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 14:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 15:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 16:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 17:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 18:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 19:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 20:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 21:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 22:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 23:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 24:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 25:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 26:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

}


	}
	else
	{
		info.att = new String();
		match = -1;
	}

	return match;
}

function infoClass()
{
	var offset; var src; var att; 
}

function __MathAsmCCparse( src, err_off, err_la )
{
	var		sstack			= new Array();
	var		vstack			= new Array();
	var 	err_cnt			= 0;
	var		act;
	var		go;
	var		la;
	var		rval;
	//var 	parseinfo		= new Function( "", "var offset; var src; var att;" );
	var		info			= new infoClass(); // new parseinfo();
	
/* Pop-Table */
var pop_tab = new Array(
	new Array( 0/* Program' */, 1 ),
	new Array( 27/* Program */, 2 ),
	new Array( 27/* Program */, 0 ),
	new Array( 28/* Instruction */, 2 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 ),
	new Array( 29/* UnaryInstr */, 1 )
);

/* Action-Table */
var act_tab = new Array(
	/* State 0 */ new Array( 30/* "$" */,-2 , 4/* "Pow" */,-2 , 5/* "Sqr" */,-2 , 6/* "Sqrt" */,-2 , 7/* "Exp" */,-2 , 8/* "Sin" */,-2 , 9/* "Cos" */,-2 , 10/* "Tan" */,-2 , 11/* "Asin" */,-2 , 12/* "Acos" */,-2 , 13/* "Atan" */,-2 , 14/* "Atan2" */,-2 , 15/* "Sinh" */,-2 , 16/* "Cosh" */,-2 , 17/* "Tanh" */,-2 , 18/* "Hypot" */,-2 , 19/* "Ln" */,-2 , 20/* "Log10" */,-2 , 21/* "Rnd" */,-2 , 22/* "Trunc" */,-2 , 23/* "Floor" */,-2 , 24/* "Ceil" */,-2 , 25/* "IsNan" */,-2 , 26/* "IsInf" */,-2 ),
	/* State 1 */ new Array( 4/* "Pow" */,4 , 5/* "Sqr" */,5 , 6/* "Sqrt" */,6 , 7/* "Exp" */,7 , 8/* "Sin" */,8 , 9/* "Cos" */,9 , 10/* "Tan" */,10 , 11/* "Asin" */,11 , 12/* "Acos" */,12 , 13/* "Atan" */,13 , 14/* "Atan2" */,14 , 15/* "Sinh" */,15 , 16/* "Cosh" */,16 , 17/* "Tanh" */,17 , 18/* "Hypot" */,18 , 19/* "Ln" */,19 , 20/* "Log10" */,20 , 21/* "Rnd" */,21 , 22/* "Trunc" */,22 , 23/* "Floor" */,23 , 24/* "Ceil" */,24 , 25/* "IsNan" */,25 , 26/* "IsInf" */,26 , 30/* "$" */,0 ),
	/* State 2 */ new Array( 30/* "$" */,-1 , 4/* "Pow" */,-1 , 5/* "Sqr" */,-1 , 6/* "Sqrt" */,-1 , 7/* "Exp" */,-1 , 8/* "Sin" */,-1 , 9/* "Cos" */,-1 , 10/* "Tan" */,-1 , 11/* "Asin" */,-1 , 12/* "Acos" */,-1 , 13/* "Atan" */,-1 , 14/* "Atan2" */,-1 , 15/* "Sinh" */,-1 , 16/* "Cosh" */,-1 , 17/* "Tanh" */,-1 , 18/* "Hypot" */,-1 , 19/* "Ln" */,-1 , 20/* "Log10" */,-1 , 21/* "Rnd" */,-1 , 22/* "Trunc" */,-1 , 23/* "Floor" */,-1 , 24/* "Ceil" */,-1 , 25/* "IsNan" */,-1 , 26/* "IsInf" */,-1 ),
	/* State 3 */ new Array( 2/* "NL" */,27 ),
	/* State 4 */ new Array( 2/* "NL" */,-4 ),
	/* State 5 */ new Array( 2/* "NL" */,-5 ),
	/* State 6 */ new Array( 2/* "NL" */,-6 ),
	/* State 7 */ new Array( 2/* "NL" */,-7 ),
	/* State 8 */ new Array( 2/* "NL" */,-8 ),
	/* State 9 */ new Array( 2/* "NL" */,-9 ),
	/* State 10 */ new Array( 2/* "NL" */,-10 ),
	/* State 11 */ new Array( 2/* "NL" */,-11 ),
	/* State 12 */ new Array( 2/* "NL" */,-12 ),
	/* State 13 */ new Array( 2/* "NL" */,-13 ),
	/* State 14 */ new Array( 2/* "NL" */,-14 ),
	/* State 15 */ new Array( 2/* "NL" */,-15 ),
	/* State 16 */ new Array( 2/* "NL" */,-16 ),
	/* State 17 */ new Array( 2/* "NL" */,-17 ),
	/* State 18 */ new Array( 2/* "NL" */,-18 ),
	/* State 19 */ new Array( 2/* "NL" */,-19 ),
	/* State 20 */ new Array( 2/* "NL" */,-20 ),
	/* State 21 */ new Array( 2/* "NL" */,-21 ),
	/* State 22 */ new Array( 2/* "NL" */,-22 ),
	/* State 23 */ new Array( 2/* "NL" */,-23 ),
	/* State 24 */ new Array( 2/* "NL" */,-24 ),
	/* State 25 */ new Array( 2/* "NL" */,-25 ),
	/* State 26 */ new Array( 2/* "NL" */,-26 ),
	/* State 27 */ new Array( 30/* "$" */,-3 , 4/* "Pow" */,-3 , 5/* "Sqr" */,-3 , 6/* "Sqrt" */,-3 , 7/* "Exp" */,-3 , 8/* "Sin" */,-3 , 9/* "Cos" */,-3 , 10/* "Tan" */,-3 , 11/* "Asin" */,-3 , 12/* "Acos" */,-3 , 13/* "Atan" */,-3 , 14/* "Atan2" */,-3 , 15/* "Sinh" */,-3 , 16/* "Cosh" */,-3 , 17/* "Tanh" */,-3 , 18/* "Hypot" */,-3 , 19/* "Ln" */,-3 , 20/* "Log10" */,-3 , 21/* "Rnd" */,-3 , 22/* "Trunc" */,-3 , 23/* "Floor" */,-3 , 24/* "Ceil" */,-3 , 25/* "IsNan" */,-3 , 26/* "IsInf" */,-3 )
);

/* Goto-Table */
var goto_tab = new Array(
	/* State 0 */ new Array( 27/* Program */,1 ),
	/* State 1 */ new Array( 28/* Instruction */,2 , 29/* UnaryInstr */,3 ),
	/* State 2 */ new Array(  ),
	/* State 3 */ new Array(  ),
	/* State 4 */ new Array(  ),
	/* State 5 */ new Array(  ),
	/* State 6 */ new Array(  ),
	/* State 7 */ new Array(  ),
	/* State 8 */ new Array(  ),
	/* State 9 */ new Array(  ),
	/* State 10 */ new Array(  ),
	/* State 11 */ new Array(  ),
	/* State 12 */ new Array(  ),
	/* State 13 */ new Array(  ),
	/* State 14 */ new Array(  ),
	/* State 15 */ new Array(  ),
	/* State 16 */ new Array(  ),
	/* State 17 */ new Array(  ),
	/* State 18 */ new Array(  ),
	/* State 19 */ new Array(  ),
	/* State 20 */ new Array(  ),
	/* State 21 */ new Array(  ),
	/* State 22 */ new Array(  ),
	/* State 23 */ new Array(  ),
	/* State 24 */ new Array(  ),
	/* State 25 */ new Array(  ),
	/* State 26 */ new Array(  ),
	/* State 27 */ new Array(  )
);



/* Symbol labels */
var labels = new Array(
	"Program'" /* Non-terminal symbol */,
	"WHITESPACE" /* Terminal symbol */,
	"NL" /* Terminal symbol */,
	"math" /* Terminal symbol */,
	"Pow" /* Terminal symbol */,
	"Sqr" /* Terminal symbol */,
	"Sqrt" /* Terminal symbol */,
	"Exp" /* Terminal symbol */,
	"Sin" /* Terminal symbol */,
	"Cos" /* Terminal symbol */,
	"Tan" /* Terminal symbol */,
	"Asin" /* Terminal symbol */,
	"Acos" /* Terminal symbol */,
	"Atan" /* Terminal symbol */,
	"Atan2" /* Terminal symbol */,
	"Sinh" /* Terminal symbol */,
	"Cosh" /* Terminal symbol */,
	"Tanh" /* Terminal symbol */,
	"Hypot" /* Terminal symbol */,
	"Ln" /* Terminal symbol */,
	"Log10" /* Terminal symbol */,
	"Rnd" /* Terminal symbol */,
	"Trunc" /* Terminal symbol */,
	"Floor" /* Terminal symbol */,
	"Ceil" /* Terminal symbol */,
	"IsNan" /* Terminal symbol */,
	"IsInf" /* Terminal symbol */,
	"Program" /* Non-terminal symbol */,
	"Instruction" /* Non-terminal symbol */,
	"UnaryInstr" /* Non-terminal symbol */,
	"$" /* Terminal symbol */
);


	
	info.offset = 0;
	info.src = src;
	info.att = new String();
	
	if( !err_off )
		err_off	= new Array();
	if( !err_la )
	err_la = new Array();
	
	sstack.push( 0 );
	vstack.push( 0 );
	
	la = __MathAsmCClex( info );

	while( true )
	{
		act = 29;
		for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
		{
			if( act_tab[sstack[sstack.length-1]][i] == la )
			{
				act = act_tab[sstack[sstack.length-1]][i+1];
				break;
			}
		}

		if( MathAsmCC_dbg_withtrace && sstack.length > 0 )
		{
			__MathAsmCCdbg_print( "\nState " + sstack[sstack.length-1] + "\n" +
							"\tLookahead: " + labels[la] + " (\"" + info.att + "\")\n" +
							"\tAction: " + act + "\n" + 
							"\tSource: \"" + info.src.substr( info.offset, 30 ) + ( ( info.offset + 30 < info.src.length ) ?
									"..." : "" ) + "\"\n" +
							"\tStack: " + sstack.join() + "\n" +
							"\tValue stack: " + vstack.join() + "\n" );
		}
		
			
		//Panic-mode: Try recovery when parse-error occurs!
		if( act == 29 )
		{
			if( MathAsmCC_dbg_withtrace )
				__MathAsmCCdbg_print( "Error detected: There is no reduce or shift on the symbol " + labels[la] );
			
			err_cnt++;
			err_off.push( info.offset - info.att.length );			
			err_la.push( new Array() );
			for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
				err_la[err_la.length-1].push( labels[act_tab[sstack[sstack.length-1]][i]] );
			
			//Remember the original stack!
			var rsstack = new Array();
			var rvstack = new Array();
			for( var i = 0; i < sstack.length; i++ )
			{
				rsstack[i] = sstack[i];
				rvstack[i] = vstack[i];
			}
			
			while( act == 29 && la != 30 )
			{
				if( MathAsmCC_dbg_withtrace )
					__MathAsmCCdbg_print( "\tError recovery\n" +
									"Current lookahead: " + labels[la] + " (" + info.att + ")\n" +
									"Action: " + act + "\n\n" );
				if( la == -1 )
					info.offset++;
					
				while( act == 29 && sstack.length > 0 )
				{
					sstack.pop();
					vstack.pop();
					
					if( sstack.length == 0 )
						break;
						
					act = 29;
					for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
					{
						if( act_tab[sstack[sstack.length-1]][i] == la )
						{
							act = act_tab[sstack[sstack.length-1]][i+1];
							break;
						}
					}
				}
				
				if( act != 29 )
					break;
				
				for( var i = 0; i < rsstack.length; i++ )
				{
					sstack.push( rsstack[i] );
					vstack.push( rvstack[i] );
				}
				
				la = __MathAsmCClex( info );
			}
			
			if( act == 29 )
			{
				if( MathAsmCC_dbg_withtrace )
					__MathAsmCCdbg_print( "\tError recovery failed, terminating parse process..." );
				break;
			}


			if( MathAsmCC_dbg_withtrace )
				__MathAsmCCdbg_print( "\tError recovery succeeded, continuing" );
		}
		
		/*
		if( act == 29 )
			break;
		*/
		
		
		//Shift
		if( act > 0 )
		{			
			if( MathAsmCC_dbg_withtrace )
				__MathAsmCCdbg_print( "Shifting symbol: " + labels[la] + " (" + info.att + ")" );
		
			sstack.push( act );
			vstack.push( info.att );
			
			la = __MathAsmCClex( info );
			
			if( MathAsmCC_dbg_withtrace )
				__MathAsmCCdbg_print( "\tNew lookahead symbol: " + labels[la] + " (" + info.att + ")" );
		}
		//Reduce
		else
		{		
			act *= -1;
			
			if( MathAsmCC_dbg_withtrace )
				__MathAsmCCdbg_print( "Reducing by producution: " + act );
			
			rval = void(0);
			
			if( MathAsmCC_dbg_withtrace )
				__MathAsmCCdbg_print( "\tPerforming semantic action..." );
			
switch( act )
{
	case 0:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 1:
	{
		  if (vstack[ vstack.length - 1 ] !== undefined && vstack[ vstack.length - 1 ] != null)
                                                {
                                                    _ast.appendNode(vstack[ vstack.length - 1 ]);
                                                }
                                            
	}
	break;
	case 2:
	{
		rval = vstack[ vstack.length - 0 ];
	}
	break;
	case 3:
	{
		 rval = new InstructionNode(vstack[ vstack.length - 2 ]); 
	}
	break;
	case 4:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 5:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 6:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 7:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 8:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 9:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 10:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 11:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 12:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 13:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 14:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 15:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 16:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 17:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 18:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 19:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 20:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 21:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 22:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 23:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 24:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 25:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 26:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
}



			if( MathAsmCC_dbg_withtrace )
				__MathAsmCCdbg_print( "\tPopping " + pop_tab[act][1] + " off the stack..." );
				
			for( var i = 0; i < pop_tab[act][1]; i++ )
			{
				sstack.pop();
				vstack.pop();
			}
									
			go = -1;
			for( var i = 0; i < goto_tab[sstack[sstack.length-1]].length; i+=2 )
			{
				if( goto_tab[sstack[sstack.length-1]][i] == pop_tab[act][0] )
				{
					go = goto_tab[sstack[sstack.length-1]][i+1];
					break;
				}
			}
			
			if( act == 0 )
				break;
				
			if( MathAsmCC_dbg_withtrace )
				__MathAsmCCdbg_print( "\tPushing non-terminal " + labels[ pop_tab[act][0] ] );
				
			sstack.push( go );
			vstack.push( rval );			
		}
		
		if( MathAsmCC_dbg_withtrace )
		{		
			alert( MathAsmCC_dbg_string );
			MathAsmCC_dbg_string = new String();
		}
	}

	if( MathAsmCC_dbg_withtrace )
	{
		__MathAsmCCdbg_print( "\nParse complete." );
		alert( MathAsmCC_dbg_string );
	}
	
	return err_cnt;
}



module.exports.parse = __MathAsmCCparse;
module.exports.ast   = _ast;

