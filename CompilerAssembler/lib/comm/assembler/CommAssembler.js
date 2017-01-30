

//--------------------------------------------------------------------------
// My stuff

AST               = require('../../../common/Ast');
InstructionNode   = require('../../../assembler/AstNodes/InstructionNode');


var _ast = new AST.AbstractSyntaxTree();
var lib = "comm.";

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

var CommAsmCC_dbg_withtrace		= false;
var CommAsmCC_dbg_string			= new String();

function alert (text)
{
	console.log(text);
}

function __CommAsmCCdbg_print( text )
{
	CommAsmCC_dbg_string += text + "\n";
}

function __CommAsmCClex( info )
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
			return 35;

		do
		{

switch( state )
{
	case 0:
		if( info.src.charCodeAt( pos ) == 9 || info.src.charCodeAt( pos ) == 13 || info.src.charCodeAt( pos ) == 32 ) state = 1;
		else if( info.src.charCodeAt( pos ) == 10 ) state = 2;
		else if( info.src.charCodeAt( pos ) == 59 ) state = 32;
		else if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 33;
		else if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 34;
		else if( info.src.charCodeAt( pos ) == 87 || info.src.charCodeAt( pos ) == 119 ) state = 35;
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
		match = 3;
		match_pos = pos;
		break;

	case 4:
		state = -1;
		match = 18;
		match_pos = pos;
		break;

	case 5:
		state = -1;
		match = 17;
		match_pos = pos;
		break;

	case 6:
		state = -1;
		match = 19;
		match_pos = pos;
		break;

	case 7:
		state = -1;
		match = 12;
		match_pos = pos;
		break;

	case 8:
		state = -1;
		match = 10;
		match_pos = pos;
		break;

	case 9:
		state = -1;
		match = 11;
		match_pos = pos;
		break;

	case 10:
		state = -1;
		match = 13;
		match_pos = pos;
		break;

	case 11:
		if( info.src.charCodeAt( pos ) == 46 ) state = 93;
		else state = -1;
		match = 5;
		match_pos = pos;
		break;

	case 12:
		state = -1;
		match = 21;
		match_pos = pos;
		break;

	case 13:
		state = -1;
		match = 15;
		match_pos = pos;
		break;

	case 14:
		state = -1;
		match = 14;
		match_pos = pos;
		break;

	case 15:
		state = -1;
		match = 20;
		match_pos = pos;
		break;

	case 16:
		state = -1;
		match = 4;
		match_pos = pos;
		break;

	case 17:
		state = -1;
		match = 6;
		match_pos = pos;
		break;

	case 18:
		state = -1;
		match = 7;
		match_pos = pos;
		break;

	case 19:
		state = -1;
		match = 16;
		match_pos = pos;
		break;

	case 20:
		state = -1;
		match = 25;
		match_pos = pos;
		break;

	case 21:
		state = -1;
		match = 9;
		match_pos = pos;
		break;

	case 22:
		state = -1;
		match = 28;
		match_pos = pos;
		break;

	case 23:
		state = -1;
		match = 26;
		match_pos = pos;
		break;

	case 24:
		state = -1;
		match = 27;
		match_pos = pos;
		break;

	case 25:
		state = -1;
		match = 30;
		match_pos = pos;
		break;

	case 26:
		state = -1;
		match = 29;
		match_pos = pos;
		break;

	case 27:
		state = -1;
		match = 31;
		match_pos = pos;
		break;

	case 28:
		state = -1;
		match = 22;
		match_pos = pos;
		break;

	case 29:
		state = -1;
		match = 8;
		match_pos = pos;
		break;

	case 30:
		state = -1;
		match = 24;
		match_pos = pos;
		break;

	case 31:
		state = -1;
		match = 23;
		match_pos = pos;
		break;

	case 32:
		if( info.src.charCodeAt( pos ) == 10 ) state = 2;
		else if( ( info.src.charCodeAt( pos ) >= 0 && info.src.charCodeAt( pos ) <= 9 ) || ( info.src.charCodeAt( pos ) >= 11 && info.src.charCodeAt( pos ) <= 254 ) ) state = 32;
		else state = -1;
		break;

	case 33:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 36;
		else state = -1;
		break;

	case 34:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 37;
		else state = -1;
		break;

	case 35:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 38;
		else state = -1;
		break;

	case 36:
		if( info.src.charCodeAt( pos ) == 77 || info.src.charCodeAt( pos ) == 109 ) state = 39;
		else state = -1;
		break;

	case 37:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 40;
		else state = -1;
		break;

	case 38:
		if( info.src.charCodeAt( pos ) == 70 || info.src.charCodeAt( pos ) == 102 ) state = 134;
		else state = -1;
		break;

	case 39:
		if( info.src.charCodeAt( pos ) == 77 || info.src.charCodeAt( pos ) == 109 ) state = 3;
		else state = -1;
		break;

	case 40:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 41;
		else state = -1;
		break;

	case 41:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 43;
		else state = -1;
		break;

	case 42:
		if( info.src.charCodeAt( pos ) == 46 ) state = 44;
		else state = -1;
		break;

	case 43:
		if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 45;
		else state = -1;
		break;

	case 44:
		if( info.src.charCodeAt( pos ) == 66 || info.src.charCodeAt( pos ) == 98 ) state = 46;
		else if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 47;
		else if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 48;
		else if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 49;
		else if( info.src.charCodeAt( pos ) == 71 || info.src.charCodeAt( pos ) == 103 ) state = 50;
		else if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 51;
		else if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 52;
		else if( info.src.charCodeAt( pos ) == 77 || info.src.charCodeAt( pos ) == 109 ) state = 137;
		else if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 167;
		else state = -1;
		break;

	case 45:
		if( info.src.charCodeAt( pos ) == 46 ) state = 53;
		else state = -1;
		break;

	case 46:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 135;
		else state = -1;
		break;

	case 47:
		if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 54;
		else if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 136;
		else state = -1;
		break;

	case 48:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 154;
		else state = -1;
		break;

	case 49:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 138;
		else state = -1;
		break;

	case 50:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 55;
		else state = -1;
		break;

	case 51:
		if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 4;
		else state = -1;
		break;

	case 52:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 57;
		else if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 58;
		else if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 139;
		else if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 140;
		else state = -1;
		break;

	case 53:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 59;
		else state = -1;
		break;

	case 54:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 60;
		else state = -1;
		break;

	case 55:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 142;
		else state = -1;
		break;

	case 56:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 5;
		else if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 63;
		else state = -1;
		break;

	case 57:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 65;
		else state = -1;
		break;

	case 58:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 66;
		else state = -1;
		break;

	case 59:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 69;
		else state = -1;
		break;

	case 60:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 143;
		else state = -1;
		break;

	case 61:
		if( info.src.charCodeAt( pos ) == 70 || info.src.charCodeAt( pos ) == 102 ) state = 71;
		else if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 72;
		else state = -1;
		break;

	case 62:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 73;
		else state = -1;
		break;

	case 63:
		if( info.src.charCodeAt( pos ) == 75 || info.src.charCodeAt( pos ) == 107 ) state = 6;
		else state = -1;
		break;

	case 64:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 7;
		else state = -1;
		break;

	case 65:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 8;
		else state = -1;
		break;

	case 66:
		if( info.src.charCodeAt( pos ) == 86 || info.src.charCodeAt( pos ) == 118 ) state = 144;
		else state = -1;
		break;

	case 67:
		if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 9;
		else state = -1;
		break;

	case 68:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 76;
		else state = -1;
		break;

	case 69:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 77;
		else state = -1;
		break;

	case 70:
		if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 10;
		else state = -1;
		break;

	case 71:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 79;
		else state = -1;
		break;

	case 72:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 80;
		else state = -1;
		break;

	case 73:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 81;
		else state = -1;
		break;

	case 74:
		if( info.src.charCodeAt( pos ) == 89 || info.src.charCodeAt( pos ) == 121 ) state = 82;
		else state = -1;
		break;

	case 75:
		if( info.src.charCodeAt( pos ) == 87 || info.src.charCodeAt( pos ) == 119 ) state = 83;
		else state = -1;
		break;

	case 76:
		if( info.src.charCodeAt( pos ) == 85 || info.src.charCodeAt( pos ) == 117 ) state = 85;
		else state = -1;
		break;

	case 77:
		if( info.src.charCodeAt( pos ) == 70 || info.src.charCodeAt( pos ) == 102 ) state = 86;
		else state = -1;
		break;

	case 78:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 87;
		else state = -1;
		break;

	case 79:
		if( info.src.charCodeAt( pos ) == 71 || info.src.charCodeAt( pos ) == 103 ) state = 11;
		else state = -1;
		break;

	case 80:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 88;
		else state = -1;
		break;

	case 81:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 145;
		else state = -1;
		break;

	case 82:
		if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 89;
		else state = -1;
		break;

	case 83:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 90;
		else state = -1;
		break;

	case 84:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 146;
		else state = -1;
		break;

	case 85:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 12;
		else state = -1;
		break;

	case 86:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 91;
		else state = -1;
		break;

	case 87:
		if( info.src.charCodeAt( pos ) == 46 ) state = 92;
		else state = -1;
		break;

	case 88:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 13;
		else state = -1;
		break;

	case 89:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 14;
		else state = -1;
		break;

	case 90:
		if( info.src.charCodeAt( pos ) == 89 || info.src.charCodeAt( pos ) == 121 ) state = 15;
		else state = -1;
		break;

	case 91:
		if( info.src.charCodeAt( pos ) == 71 || info.src.charCodeAt( pos ) == 103 ) state = 16;
		else state = -1;
		break;

	case 92:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 96;
		else if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 97;
		else if( info.src.charCodeAt( pos ) == 87 || info.src.charCodeAt( pos ) == 119 ) state = 98;
		else if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 147;
		else if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 168;
		else state = -1;
		break;

	case 93:
		if( info.src.charCodeAt( pos ) == 71 || info.src.charCodeAt( pos ) == 103 ) state = 99;
		else if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 100;
		else if( info.src.charCodeAt( pos ) == 77 || info.src.charCodeAt( pos ) == 109 ) state = 101;
		else if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 156;
		else state = -1;
		break;

	case 94:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 102;
		else state = -1;
		break;

	case 95:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 103;
		else if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 171;
		else if( info.src.charCodeAt( pos ) == 87 || info.src.charCodeAt( pos ) == 119 ) state = 172;
		else state = -1;
		break;

	case 96:
		if( info.src.charCodeAt( pos ) == 86 || info.src.charCodeAt( pos ) == 118 ) state = 148;
		else state = -1;
		break;

	case 97:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 160;
		else state = -1;
		break;

	case 98:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 104;
		else state = -1;
		break;

	case 99:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 149;
		else state = -1;
		break;

	case 100:
		if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 17;
		else state = -1;
		break;

	case 101:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 163;
		else state = -1;
		break;

	case 102:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 106;
		else state = -1;
		break;

	case 103:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 107;
		else state = -1;
		break;

	case 104:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 110;
		else state = -1;
		break;

	case 105:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 18;
		else state = -1;
		break;

	case 106:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 19;
		else state = -1;
		break;

	case 107:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 113;
		else if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 114;
		else state = -1;
		break;

	case 108:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 169;
		else state = -1;
		break;

	case 109:
		if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 20;
		else state = -1;
		break;

	case 110:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 116;
		else state = -1;
		break;

	case 111:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 117;
		else state = -1;
		break;

	case 112:
		if( info.src.charCodeAt( pos ) == 75 || info.src.charCodeAt( pos ) == 107 ) state = 21;
		else state = -1;
		break;

	case 113:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 119;
		else if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 120;
		else state = -1;
		break;

	case 114:
		if( info.src.charCodeAt( pos ) == 80 || info.src.charCodeAt( pos ) == 112 ) state = 22;
		else state = -1;
		break;

	case 115:
		if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 122;
		else state = -1;
		break;

	case 116:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 23;
		else state = -1;
		break;

	case 117:
		if( info.src.charCodeAt( pos ) == 87 || info.src.charCodeAt( pos ) == 119 ) state = 153;
		else state = -1;
		break;

	case 118:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 124;
		else state = -1;
		break;

	case 119:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 24;
		else state = -1;
		break;

	case 120:
		if( info.src.charCodeAt( pos ) == 85 || info.src.charCodeAt( pos ) == 117 ) state = 125;
		else state = -1;
		break;

	case 121:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 25;
		else state = -1;
		break;

	case 122:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 126;
		else state = -1;
		break;

	case 123:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 127;
		else state = -1;
		break;

	case 124:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 26;
		else state = -1;
		break;

	case 125:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 27;
		else state = -1;
		break;

	case 126:
		if( info.src.charCodeAt( pos ) == 66 || info.src.charCodeAt( pos ) == 98 ) state = 129;
		else state = -1;
		break;

	case 127:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 28;
		else state = -1;
		break;

	case 128:
		if( info.src.charCodeAt( pos ) == 89 || info.src.charCodeAt( pos ) == 121 ) state = 29;
		else state = -1;
		break;

	case 129:
		if( info.src.charCodeAt( pos ) == 76 || info.src.charCodeAt( pos ) == 108 ) state = 131;
		else state = -1;
		break;

	case 130:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 132;
		else state = -1;
		break;

	case 131:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 30;
		else state = -1;
		break;

	case 132:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 133;
		else state = -1;
		break;

	case 133:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 31;
		else state = -1;
		break;

	case 134:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 42;
		else state = -1;
		break;

	case 135:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 155;
		else state = -1;
		break;

	case 136:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 61;
		else state = -1;
		break;

	case 137:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 56;
		else state = -1;
		break;

	case 138:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 141;
		else state = -1;
		break;

	case 139:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 67;
		else state = -1;
		break;

	case 140:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 68;
		else state = -1;
		break;

	case 141:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 74;
		else state = -1;
		break;

	case 142:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 75;
		else state = -1;
		break;

	case 143:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 78;
		else state = -1;
		break;

	case 144:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 84;
		else state = -1;
		break;

	case 145:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 94;
		else state = -1;
		break;

	case 146:
		if( info.src.charCodeAt( pos ) == 46 ) state = 95;
		else state = -1;
		break;

	case 147:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 157;
		else state = -1;
		break;

	case 148:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 150;
		else state = -1;
		break;

	case 149:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 111;
		else state = -1;
		break;

	case 150:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 115;
		else state = -1;
		break;

	case 151:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 118;
		else state = -1;
		break;

	case 152:
		if( info.src.charCodeAt( pos ) == 69 || info.src.charCodeAt( pos ) == 101 ) state = 123;
		else state = -1;
		break;

	case 153:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 128;
		else state = -1;
		break;

	case 154:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 62;
		else state = -1;
		break;

	case 155:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 70;
		else state = -1;
		break;

	case 156:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 105;
		else state = -1;
		break;

	case 157:
		if( info.src.charCodeAt( pos ) == 65 || info.src.charCodeAt( pos ) == 97 ) state = 109;
		else state = -1;
		break;

	case 158:
		if( info.src.charCodeAt( pos ) == 84 || info.src.charCodeAt( pos ) == 116 ) state = 121;
		else state = -1;
		break;

	case 159:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 64;
		else state = -1;
		break;

	case 160:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 162;
		else state = -1;
		break;

	case 161:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 108;
		else state = -1;
		break;

	case 162:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 152;
		else state = -1;
		break;

	case 163:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 112;
		else state = -1;
		break;

	case 164:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 166;
		else state = -1;
		break;

	case 165:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 151;
		else state = -1;
		break;

	case 166:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 130;
		else state = -1;
		break;

	case 167:
		if( info.src.charCodeAt( pos ) == 83 || info.src.charCodeAt( pos ) == 115 ) state = 159;
		else state = -1;
		break;

	case 168:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 161;
		else state = -1;
		break;

	case 169:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 164;
		else state = -1;
		break;

	case 170:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 158;
		else state = -1;
		break;

	case 171:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 165;
		else state = -1;
		break;

	case 172:
		if( info.src.charCodeAt( pos ) == 82 || info.src.charCodeAt( pos ) == 114 ) state = 170;
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
		 info.att = { value: info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
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

	case 27:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 28:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 29:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 30:
		{
		 info.att = { value: lib + info.att, token: info.att, offset: ( info.offset - info.att.length )}; 
		}
		break;

	case 31:
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

function __CommAsmCCparse( src, err_off, err_la )
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
	new Array( 32/* Program */, 2 ),
	new Array( 32/* Program */, 0 ),
	new Array( 33/* Stmt */, 2 ),
	new Array( 33/* Stmt */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 ),
	new Array( 34/* UnaryInstr */, 1 )
);

/* Action-Table */
var act_tab = new Array(
	/* State 0 */ new Array( 35/* "$" */,-2 , 2/* "NL" */,-2 , 4/* "SerialConfig" */,-2 , 5/* "WifiConfig" */,-2 , 6/* "WifiConfigIp" */,-2 , 7/* "WifiConfigDns" */,-2 , 8/* "WifiConfigGateway" */,-2 , 9/* "WifiConfigMask" */,-2 , 10/* "WifiScan" */,-2 , 11/* "WifiSsid" */,-2 , 12/* "WifiRssi" */,-2 , 13/* "WifiBssid" */,-2 , 14/* "WifiEncrypt" */,-2 , 15/* "WifiConnect" */,-2 , 16/* "WifiDisconnect" */,-2 , 17/* "WifiMac" */,-2 , 18/* "WifiIp" */,-2 , 19/* "WifiMask" */,-2 , 20/* "WifiGateway" */,-2 , 21/* "WifiStatus" */,-2 , 22/* "WifiClientConnect" */,-2 , 23/* "WifiClientDisconnect" */,-2 , 24/* "WifiClientAvailable" */,-2 , 25/* "WifiClientRead" */,-2 , 26/* "WifiClientWrite" */,-2 , 27/* "WifiServerStart" */,-2 , 28/* "WifiServerStop" */,-2 , 29/* "WifiServerListen" */,-2 , 30/* "WifiServerWrite" */,-2 , 31/* "WifiServerStatus" */,-2 ),
	/* State 1 */ new Array( 2/* "NL" */,4 , 4/* "SerialConfig" */,5 , 5/* "WifiConfig" */,6 , 6/* "WifiConfigIp" */,7 , 7/* "WifiConfigDns" */,8 , 8/* "WifiConfigGateway" */,9 , 9/* "WifiConfigMask" */,10 , 10/* "WifiScan" */,11 , 11/* "WifiSsid" */,12 , 12/* "WifiRssi" */,13 , 13/* "WifiBssid" */,14 , 14/* "WifiEncrypt" */,15 , 15/* "WifiConnect" */,16 , 16/* "WifiDisconnect" */,17 , 17/* "WifiMac" */,18 , 18/* "WifiIp" */,19 , 19/* "WifiMask" */,20 , 20/* "WifiGateway" */,21 , 21/* "WifiStatus" */,22 , 22/* "WifiClientConnect" */,23 , 23/* "WifiClientDisconnect" */,24 , 24/* "WifiClientAvailable" */,25 , 25/* "WifiClientRead" */,26 , 26/* "WifiClientWrite" */,27 , 27/* "WifiServerStart" */,28 , 28/* "WifiServerStop" */,29 , 29/* "WifiServerListen" */,30 , 30/* "WifiServerWrite" */,31 , 31/* "WifiServerStatus" */,32 , 35/* "$" */,0 ),
	/* State 2 */ new Array( 35/* "$" */,-1 , 2/* "NL" */,-1 , 4/* "SerialConfig" */,-1 , 5/* "WifiConfig" */,-1 , 6/* "WifiConfigIp" */,-1 , 7/* "WifiConfigDns" */,-1 , 8/* "WifiConfigGateway" */,-1 , 9/* "WifiConfigMask" */,-1 , 10/* "WifiScan" */,-1 , 11/* "WifiSsid" */,-1 , 12/* "WifiRssi" */,-1 , 13/* "WifiBssid" */,-1 , 14/* "WifiEncrypt" */,-1 , 15/* "WifiConnect" */,-1 , 16/* "WifiDisconnect" */,-1 , 17/* "WifiMac" */,-1 , 18/* "WifiIp" */,-1 , 19/* "WifiMask" */,-1 , 20/* "WifiGateway" */,-1 , 21/* "WifiStatus" */,-1 , 22/* "WifiClientConnect" */,-1 , 23/* "WifiClientDisconnect" */,-1 , 24/* "WifiClientAvailable" */,-1 , 25/* "WifiClientRead" */,-1 , 26/* "WifiClientWrite" */,-1 , 27/* "WifiServerStart" */,-1 , 28/* "WifiServerStop" */,-1 , 29/* "WifiServerListen" */,-1 , 30/* "WifiServerWrite" */,-1 , 31/* "WifiServerStatus" */,-1 ),
	/* State 3 */ new Array( 2/* "NL" */,33 ),
	/* State 4 */ new Array( 35/* "$" */,-4 , 2/* "NL" */,-4 , 4/* "SerialConfig" */,-4 , 5/* "WifiConfig" */,-4 , 6/* "WifiConfigIp" */,-4 , 7/* "WifiConfigDns" */,-4 , 8/* "WifiConfigGateway" */,-4 , 9/* "WifiConfigMask" */,-4 , 10/* "WifiScan" */,-4 , 11/* "WifiSsid" */,-4 , 12/* "WifiRssi" */,-4 , 13/* "WifiBssid" */,-4 , 14/* "WifiEncrypt" */,-4 , 15/* "WifiConnect" */,-4 , 16/* "WifiDisconnect" */,-4 , 17/* "WifiMac" */,-4 , 18/* "WifiIp" */,-4 , 19/* "WifiMask" */,-4 , 20/* "WifiGateway" */,-4 , 21/* "WifiStatus" */,-4 , 22/* "WifiClientConnect" */,-4 , 23/* "WifiClientDisconnect" */,-4 , 24/* "WifiClientAvailable" */,-4 , 25/* "WifiClientRead" */,-4 , 26/* "WifiClientWrite" */,-4 , 27/* "WifiServerStart" */,-4 , 28/* "WifiServerStop" */,-4 , 29/* "WifiServerListen" */,-4 , 30/* "WifiServerWrite" */,-4 , 31/* "WifiServerStatus" */,-4 ),
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
	/* State 27 */ new Array( 2/* "NL" */,-27 ),
	/* State 28 */ new Array( 2/* "NL" */,-28 ),
	/* State 29 */ new Array( 2/* "NL" */,-29 ),
	/* State 30 */ new Array( 2/* "NL" */,-30 ),
	/* State 31 */ new Array( 2/* "NL" */,-31 ),
	/* State 32 */ new Array( 2/* "NL" */,-32 ),
	/* State 33 */ new Array( 35/* "$" */,-3 , 2/* "NL" */,-3 , 4/* "SerialConfig" */,-3 , 5/* "WifiConfig" */,-3 , 6/* "WifiConfigIp" */,-3 , 7/* "WifiConfigDns" */,-3 , 8/* "WifiConfigGateway" */,-3 , 9/* "WifiConfigMask" */,-3 , 10/* "WifiScan" */,-3 , 11/* "WifiSsid" */,-3 , 12/* "WifiRssi" */,-3 , 13/* "WifiBssid" */,-3 , 14/* "WifiEncrypt" */,-3 , 15/* "WifiConnect" */,-3 , 16/* "WifiDisconnect" */,-3 , 17/* "WifiMac" */,-3 , 18/* "WifiIp" */,-3 , 19/* "WifiMask" */,-3 , 20/* "WifiGateway" */,-3 , 21/* "WifiStatus" */,-3 , 22/* "WifiClientConnect" */,-3 , 23/* "WifiClientDisconnect" */,-3 , 24/* "WifiClientAvailable" */,-3 , 25/* "WifiClientRead" */,-3 , 26/* "WifiClientWrite" */,-3 , 27/* "WifiServerStart" */,-3 , 28/* "WifiServerStop" */,-3 , 29/* "WifiServerListen" */,-3 , 30/* "WifiServerWrite" */,-3 , 31/* "WifiServerStatus" */,-3 )
);

/* Goto-Table */
var goto_tab = new Array(
	/* State 0 */ new Array( 32/* Program */,1 ),
	/* State 1 */ new Array( 33/* Stmt */,2 , 34/* UnaryInstr */,3 ),
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
	/* State 27 */ new Array(  ),
	/* State 28 */ new Array(  ),
	/* State 29 */ new Array(  ),
	/* State 30 */ new Array(  ),
	/* State 31 */ new Array(  ),
	/* State 32 */ new Array(  ),
	/* State 33 */ new Array(  )
);



/* Symbol labels */
var labels = new Array(
	"Program'" /* Non-terminal symbol */,
	"WHITESPACE" /* Terminal symbol */,
	"NL" /* Terminal symbol */,
	"comm" /* Terminal symbol */,
	"SerialConfig" /* Terminal symbol */,
	"WifiConfig" /* Terminal symbol */,
	"WifiConfigIp" /* Terminal symbol */,
	"WifiConfigDns" /* Terminal symbol */,
	"WifiConfigGateway" /* Terminal symbol */,
	"WifiConfigMask" /* Terminal symbol */,
	"WifiScan" /* Terminal symbol */,
	"WifiSsid" /* Terminal symbol */,
	"WifiRssi" /* Terminal symbol */,
	"WifiBssid" /* Terminal symbol */,
	"WifiEncrypt" /* Terminal symbol */,
	"WifiConnect" /* Terminal symbol */,
	"WifiDisconnect" /* Terminal symbol */,
	"WifiMac" /* Terminal symbol */,
	"WifiIp" /* Terminal symbol */,
	"WifiMask" /* Terminal symbol */,
	"WifiGateway" /* Terminal symbol */,
	"WifiStatus" /* Terminal symbol */,
	"WifiClientConnect" /* Terminal symbol */,
	"WifiClientDisconnect" /* Terminal symbol */,
	"WifiClientAvailable" /* Terminal symbol */,
	"WifiClientRead" /* Terminal symbol */,
	"WifiClientWrite" /* Terminal symbol */,
	"WifiServerStart" /* Terminal symbol */,
	"WifiServerStop" /* Terminal symbol */,
	"WifiServerListen" /* Terminal symbol */,
	"WifiServerWrite" /* Terminal symbol */,
	"WifiServerStatus" /* Terminal symbol */,
	"Program" /* Non-terminal symbol */,
	"Stmt" /* Non-terminal symbol */,
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
	
	la = __CommAsmCClex( info );

	while( true )
	{
		act = 35;
		for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
		{
			if( act_tab[sstack[sstack.length-1]][i] == la )
			{
				act = act_tab[sstack[sstack.length-1]][i+1];
				break;
			}
		}

		if( CommAsmCC_dbg_withtrace && sstack.length > 0 )
		{
			__CommAsmCCdbg_print( "\nState " + sstack[sstack.length-1] + "\n" +
							"\tLookahead: " + labels[la] + " (\"" + info.att + "\")\n" +
							"\tAction: " + act + "\n" + 
							"\tSource: \"" + info.src.substr( info.offset, 30 ) + ( ( info.offset + 30 < info.src.length ) ?
									"..." : "" ) + "\"\n" +
							"\tStack: " + sstack.join() + "\n" +
							"\tValue stack: " + vstack.join() + "\n" );
		}
		
			
		//Panic-mode: Try recovery when parse-error occurs!
		if( act == 35 )
		{
			if( CommAsmCC_dbg_withtrace )
				__CommAsmCCdbg_print( "Error detected: There is no reduce or shift on the symbol " + labels[la] );
			
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
			
			while( act == 35 && la != 35 )
			{
				if( CommAsmCC_dbg_withtrace )
					__CommAsmCCdbg_print( "\tError recovery\n" +
									"Current lookahead: " + labels[la] + " (" + info.att + ")\n" +
									"Action: " + act + "\n\n" );
				if( la == -1 )
					info.offset++;
					
				while( act == 35 && sstack.length > 0 )
				{
					sstack.pop();
					vstack.pop();
					
					if( sstack.length == 0 )
						break;
						
					act = 35;
					for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
					{
						if( act_tab[sstack[sstack.length-1]][i] == la )
						{
							act = act_tab[sstack[sstack.length-1]][i+1];
							break;
						}
					}
				}
				
				if( act != 35 )
					break;
				
				for( var i = 0; i < rsstack.length; i++ )
				{
					sstack.push( rsstack[i] );
					vstack.push( rvstack[i] );
				}
				
				la = __CommAsmCClex( info );
			}
			
			if( act == 35 )
			{
				if( CommAsmCC_dbg_withtrace )
					__CommAsmCCdbg_print( "\tError recovery failed, terminating parse process..." );
				break;
			}


			if( CommAsmCC_dbg_withtrace )
				__CommAsmCCdbg_print( "\tError recovery succeeded, continuing" );
		}
		
		/*
		if( act == 35 )
			break;
		*/
		
		
		//Shift
		if( act > 0 )
		{			
			if( CommAsmCC_dbg_withtrace )
				__CommAsmCCdbg_print( "Shifting symbol: " + labels[la] + " (" + info.att + ")" );
		
			sstack.push( act );
			vstack.push( info.att );
			
			la = __CommAsmCClex( info );
			
			if( CommAsmCC_dbg_withtrace )
				__CommAsmCCdbg_print( "\tNew lookahead symbol: " + labels[la] + " (" + info.att + ")" );
		}
		//Reduce
		else
		{		
			act *= -1;
			
			if( CommAsmCC_dbg_withtrace )
				__CommAsmCCdbg_print( "Reducing by producution: " + act );
			
			rval = void(0);
			
			if( CommAsmCC_dbg_withtrace )
				__CommAsmCCdbg_print( "\tPerforming semantic action..." );
			
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
	case 27:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 28:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 29:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 30:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 31:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
	case 32:
	{
		rval = vstack[ vstack.length - 1 ];
	}
	break;
}



			if( CommAsmCC_dbg_withtrace )
				__CommAsmCCdbg_print( "\tPopping " + pop_tab[act][1] + " off the stack..." );
				
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
				
			if( CommAsmCC_dbg_withtrace )
				__CommAsmCCdbg_print( "\tPushing non-terminal " + labels[ pop_tab[act][0] ] );
				
			sstack.push( go );
			vstack.push( rval );			
		}
		
		if( CommAsmCC_dbg_withtrace )
		{		
			alert( CommAsmCC_dbg_string );
			CommAsmCC_dbg_string = new String();
		}
	}

	if( CommAsmCC_dbg_withtrace )
	{
		__CommAsmCCdbg_print( "\nParse complete." );
		alert( CommAsmCC_dbg_string );
	}
	
	return err_cnt;
}



module.exports.parse = __CommAsmCCparse;
module.exports.ast   = _ast;

