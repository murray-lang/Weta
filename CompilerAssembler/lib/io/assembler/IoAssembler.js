

//--------------------------------------------------------------------------
// My stuff

AST               = require('../../../common/Ast');
InstructionNode   = require('../../../assembler/AstNodes/InstructionNode');


var _ast = new AST.AbstractSyntaxTree();
var lib = "io.";

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

var IoAsmCC_dbg_withtrace		= false;
var IoAsmCC_dbg_string			= new String();

function alert (text)
{
	console.log(text);
}

function __IoAsmCCdbg_print( text )
{
	IoAsmCC_dbg_string += text + "\n";
}

function __IoAsmCClex( info )
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
			return 8;

		do
		{

switch( state )
{
	case 0:
		if( info.src.charCodeAt( pos ) == 9 || info.src.charCodeAt( pos ) == 13 || info.src.charCodeAt( pos ) == 32 ) state = 1;
		else if( info.src.charCodeAt( pos ) == 10 ) state = 2;
		else if( info.src.charCodeAt( pos ) == 59 ) state = 5;
		else if( info.src.charCodeAt( pos ) == 68 || info.src.charCodeAt( pos ) == 100 ) state = 6;
		else if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 7;
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
		match = 4;
		match_pos = pos;
		break;

	case 5:
		if( info.src.charCodeAt( pos ) == 10 ) state = 2;
		else if( ( info.src.charCodeAt( pos ) >= 0 && info.src.charCodeAt( pos ) <= 9 ) || ( info.src.charCodeAt( pos ) >= 11 && info.src.charCodeAt( pos ) <= 254 ) ) state = 5;
		else state = -1;
		break;

	case 6:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 8;
		else state = -1;
		break;

	case 7:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 3;
		else state = -1;
		break;

	case 8:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 9;
		else state = -1;
		break;

	case 9:
		if( info.src.charCodeAt( pos ) == 46 ) state = 10;
		else state = -1;
		break;

	case 10:
		if( info.src.charCodeAt( pos ) == 67 || info.src.charCodeAt( pos ) == 99 ) state = 11;
		else state = -1;
		break;

	case 11:
		if( info.src.charCodeAt( pos ) == 79 || info.src.charCodeAt( pos ) == 111 ) state = 12;
		else state = -1;
		break;

	case 12:
		if( info.src.charCodeAt( pos ) == 78 || info.src.charCodeAt( pos ) == 110 ) state = 13;
		else state = -1;
		break;

	case 13:
		if( info.src.charCodeAt( pos ) == 70 || info.src.charCodeAt( pos ) == 102 ) state = 14;
		else state = -1;
		break;

	case 14:
		if( info.src.charCodeAt( pos ) == 73 || info.src.charCodeAt( pos ) == 105 ) state = 15;
		else state = -1;
		break;

	case 15:
		if( info.src.charCodeAt( pos ) == 71 || info.src.charCodeAt( pos ) == 103 ) state = 4;
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

function __IoAsmCCparse( src, err_off, err_la )
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
	new Array( 5/* Program */, 2 ),
	new Array( 5/* Program */, 0 ),
	new Array( 6/* Stmt */, 2 ),
	new Array( 6/* Stmt */, 1 ),
	new Array( 7/* UnaryInstr */, 1 )
);

/* Action-Table */
var act_tab = new Array(
	/* State 0 */ new Array( 8/* "$" */,-2 , 2/* "NL" */,-2 , 4/* "DinConfig" */,-2 ),
	/* State 1 */ new Array( 2/* "NL" */,4 , 4/* "DinConfig" */,5 , 8/* "$" */,0 ),
	/* State 2 */ new Array( 8/* "$" */,-1 , 2/* "NL" */,-1 , 4/* "DinConfig" */,-1 ),
	/* State 3 */ new Array( 2/* "NL" */,6 ),
	/* State 4 */ new Array( 8/* "$" */,-4 , 2/* "NL" */,-4 , 4/* "DinConfig" */,-4 ),
	/* State 5 */ new Array( 2/* "NL" */,-5 ),
	/* State 6 */ new Array( 8/* "$" */,-3 , 2/* "NL" */,-3 , 4/* "DinConfig" */,-3 )
);

/* Goto-Table */
var goto_tab = new Array(
	/* State 0 */ new Array( 5/* Program */,1 ),
	/* State 1 */ new Array( 6/* Stmt */,2 , 7/* UnaryInstr */,3 ),
	/* State 2 */ new Array(  ),
	/* State 3 */ new Array(  ),
	/* State 4 */ new Array(  ),
	/* State 5 */ new Array(  ),
	/* State 6 */ new Array(  )
);



/* Symbol labels */
var labels = new Array(
	"Program'" /* Non-terminal symbol */,
	"WHITESPACE" /* Terminal symbol */,
	"NL" /* Terminal symbol */,
	"io" /* Terminal symbol */,
	"DinConfig" /* Terminal symbol */,
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
	
	la = __IoAsmCClex( info );

	while( true )
	{
		act = 8;
		for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
		{
			if( act_tab[sstack[sstack.length-1]][i] == la )
			{
				act = act_tab[sstack[sstack.length-1]][i+1];
				break;
			}
		}

		if( IoAsmCC_dbg_withtrace && sstack.length > 0 )
		{
			__IoAsmCCdbg_print( "\nState " + sstack[sstack.length-1] + "\n" +
							"\tLookahead: " + labels[la] + " (\"" + info.att + "\")\n" +
							"\tAction: " + act + "\n" + 
							"\tSource: \"" + info.src.substr( info.offset, 30 ) + ( ( info.offset + 30 < info.src.length ) ?
									"..." : "" ) + "\"\n" +
							"\tStack: " + sstack.join() + "\n" +
							"\tValue stack: " + vstack.join() + "\n" );
		}
		
			
		//Panic-mode: Try recovery when parse-error occurs!
		if( act == 8 )
		{
			if( IoAsmCC_dbg_withtrace )
				__IoAsmCCdbg_print( "Error detected: There is no reduce or shift on the symbol " + labels[la] );
			
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
			
			while( act == 8 && la != 8 )
			{
				if( IoAsmCC_dbg_withtrace )
					__IoAsmCCdbg_print( "\tError recovery\n" +
									"Current lookahead: " + labels[la] + " (" + info.att + ")\n" +
									"Action: " + act + "\n\n" );
				if( la == -1 )
					info.offset++;
					
				while( act == 8 && sstack.length > 0 )
				{
					sstack.pop();
					vstack.pop();
					
					if( sstack.length == 0 )
						break;
						
					act = 8;
					for( var i = 0; i < act_tab[sstack[sstack.length-1]].length; i+=2 )
					{
						if( act_tab[sstack[sstack.length-1]][i] == la )
						{
							act = act_tab[sstack[sstack.length-1]][i+1];
							break;
						}
					}
				}
				
				if( act != 8 )
					break;
				
				for( var i = 0; i < rsstack.length; i++ )
				{
					sstack.push( rsstack[i] );
					vstack.push( rvstack[i] );
				}
				
				la = __IoAsmCClex( info );
			}
			
			if( act == 8 )
			{
				if( IoAsmCC_dbg_withtrace )
					__IoAsmCCdbg_print( "\tError recovery failed, terminating parse process..." );
				break;
			}


			if( IoAsmCC_dbg_withtrace )
				__IoAsmCCdbg_print( "\tError recovery succeeded, continuing" );
		}
		
		/*
		if( act == 8 )
			break;
		*/
		
		
		//Shift
		if( act > 0 )
		{			
			if( IoAsmCC_dbg_withtrace )
				__IoAsmCCdbg_print( "Shifting symbol: " + labels[la] + " (" + info.att + ")" );
		
			sstack.push( act );
			vstack.push( info.att );
			
			la = __IoAsmCClex( info );
			
			if( IoAsmCC_dbg_withtrace )
				__IoAsmCCdbg_print( "\tNew lookahead symbol: " + labels[la] + " (" + info.att + ")" );
		}
		//Reduce
		else
		{		
			act *= -1;
			
			if( IoAsmCC_dbg_withtrace )
				__IoAsmCCdbg_print( "Reducing by producution: " + act );
			
			rval = void(0);
			
			if( IoAsmCC_dbg_withtrace )
				__IoAsmCCdbg_print( "\tPerforming semantic action..." );
			
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
}



			if( IoAsmCC_dbg_withtrace )
				__IoAsmCCdbg_print( "\tPopping " + pop_tab[act][1] + " off the stack..." );
				
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
				
			if( IoAsmCC_dbg_withtrace )
				__IoAsmCCdbg_print( "\tPushing non-terminal " + labels[ pop_tab[act][0] ] );
				
			sstack.push( go );
			vstack.push( rval );			
		}
		
		if( IoAsmCC_dbg_withtrace )
		{		
			alert( IoAsmCC_dbg_string );
			IoAsmCC_dbg_string = new String();
		}
	}

	if( IoAsmCC_dbg_withtrace )
	{
		__IoAsmCCdbg_print( "\nParse complete." );
		alert( IoAsmCC_dbg_string );
	}
	
	return err_cnt;
}



module.exports.parse = __IoAsmCCparse;
module.exports.ast   = _ast;

