#include "sclPrinter.h"

//============================================================================
//  Print Lexical Token Tag
//============================================================================
void PrintTokenTag(sclLexTok* _tok, // *IN* Token to Print
				   FILE* _outFile)  // *OUT* File printed to
{
	switch(_tok->m_tag)
	{
	case sclLexTok::NULL_TYPE:	fprintf(_outFile,"----------  ");	break;
	case sclLexTok::CONST:		fprintf(_outFile,"CONST ");			break;
	case sclLexTok::LET:		fprintf(_outFile,"LET ");			break;
	case sclLexTok::IN_OP:		fprintf(_outFile,"IN ");			break;
	case sclLexTok::ASSIGN:		fprintf(_outFile,"= ");				break;
	case sclLexTok::END:		fprintf(_outFile,"\n");				break;
	case sclLexTok::FLOAT_TYPE:	fprintf(_outFile,"float ");			break;
	case sclLexTok::STRING_TYPE:fprintf(_outFile,"string ");		break;
	case sclLexTok::INT_TYPE:	fprintf(_outFile,"int ");			break;
	case sclLexTok::BOOL_TYPE:	fprintf(_outFile,"bool ");			break;
	case sclLexTok::LEFTBKT:	fprintf(_outFile,"("); 				break;
	case sclLexTok::RIGHTBKT:	fprintf(_outFile,") ");				break;
	case sclLexTok::RELOP:
	case sclLexTok::MULOP:
	case sclLexTok::ADDOP:
	case sclLexTok::NOTOP:
	case sclLexTok::BOOL_LIT:	
		{
			/*----------------------------------
				 Print Out Sub Type
			 ----------------------------------*/
			switch(_tok->m_sub)
			{
			case sclLexTok::NULL_SUB:	fprintf(_outFile,"# ");		break;
			case sclLexTok::EQUAL:		fprintf(_outFile,"== ");	break;
			case sclLexTok::NOT_EQ:		fprintf(_outFile,"!= ");	break;
			case sclLexTok::LESS:		fprintf(_outFile,"< ");		break;
			case sclLexTok::GREAT:		fprintf(_outFile,"> ");		break;
			case sclLexTok::LESS_EQ:	fprintf(_outFile,"<= ");	break;
			case sclLexTok::GREAT_EQ:	fprintf(_outFile,">= ");	break;
			case sclLexTok::ADD:		fprintf(_outFile,"+ ");		break;
			case sclLexTok::SUB:		fprintf(_outFile,"- ");		break;
			case sclLexTok::TIMES:		fprintf(_outFile,"* ");		break;
			case sclLexTok::DIV:		fprintf(_outFile,"/ ");		break;
			case sclLexTok::MOD:		fprintf(_outFile,"% ");		break;
			case sclLexTok::OR:			fprintf(_outFile,"|| ");	break;
			case sclLexTok::AND:		fprintf(_outFile,"&& ");	break;
			case sclLexTok::LIT_FALSE:	fprintf(_outFile,"false ");	break;
			case sclLexTok::LIT_TRUE:	fprintf(_outFile,"true ");	break;
			} // END OF switch(_tok->m_sub)
		}break;

	case sclLexTok::IDENT:		
		fprintf(_outFile,"%s ",(char*)(_tok->m_data));	
		break;
	case sclLexTok::STRING_LIT:	
		fprintf(_outFile,"%s ",(char*)(_tok->m_data));	
		break;
	case sclLexTok::FLOAT_LIT:	
		fprintf(_outFile,"%f ",*((float*)(_tok->m_data)));
		break;
	case sclLexTok::INT_LIT:	
		fprintf(_outFile,"%i ",*((int*)(_tok->m_data)));		
		break;
	} // END OF switch(_tok->m_tag)
}

//============================================================================
//  Print Lexical Token
//============================================================================
void PrintToken(sclLexTok* _tok, // *IN* Token to Print
				FILE* _outFile)  // *OUT* File printed to
{
	switch(_tok->m_tag)
	{
	case sclLexTok::NULL_TYPE:	fprintf(_outFile,"----------  ");	break;
	case sclLexTok::CONST:		fprintf(_outFile,"CONST       ");	break;
	case sclLexTok::LET:		fprintf(_outFile,"----LET---  ");	break;
	case sclLexTok::IN_OP:		fprintf(_outFile,"----IN----  ");	break;
	case sclLexTok::ASSIGN:		fprintf(_outFile,"ASSIGN      ");	break;
	case sclLexTok::END:		fprintf(_outFile,"--END--     ");	break;
	case sclLexTok::IDENT:		fprintf(_outFile,"IDENT       ");	break;
	case sclLexTok::FLOAT_LIT:	fprintf(_outFile,"FLOAT_LIT   ");	break;
	case sclLexTok::STRING_LIT:	fprintf(_outFile,"STRING_LIT  ");	break;
	case sclLexTok::INT_LIT:	fprintf(_outFile,"INT_LIT     ");	break;
	case sclLexTok::BOOL_LIT:	fprintf(_outFile,"BOOL_LIT    ");	break;
	case sclLexTok::FLOAT_TYPE:	fprintf(_outFile,"FLOAT_TYPE  ");	break;
	case sclLexTok::STRING_TYPE:fprintf(_outFile,"STRING_TYPE ");	break;
	case sclLexTok::INT_TYPE:	fprintf(_outFile,"INT_TYPE    ");	break;
	case sclLexTok::BOOL_TYPE:	fprintf(_outFile,"BOOL_TYPE   ");	break;
	case sclLexTok::LEFTBKT:	fprintf(_outFile,"LEFT (      "); 	break;
	case sclLexTok::RIGHTBKT:	fprintf(_outFile,"RIGHT )     ");	break;
	case sclLexTok::RELOP:		fprintf(_outFile,"REL OP      "); 	break;
	case sclLexTok::MULOP:		fprintf(_outFile,"MUL OP      "); 	break;
	case sclLexTok::ADDOP:		fprintf(_outFile,"ADD OP      "); 	break;
	case sclLexTok::NOTOP:		fprintf(_outFile,"NOT OP      "); 	break;
	} // END OF switch(_tok->m_tag)
	fputc('\t',_outFile);

	switch(_tok->m_sub)
	{
	case sclLexTok::NULL_SUB:	fprintf(_outFile,"-       ");	break;
	case sclLexTok::EQUAL:		fprintf(_outFile,"EQUAL   ");	break;
	case sclLexTok::NOT_EQ:		fprintf(_outFile,"NOT EQ  ");	break;
	case sclLexTok::LESS:		fprintf(_outFile,"LESS    ");	break;
	case sclLexTok::GREAT:		fprintf(_outFile,"GREAT   ");	break;
	case sclLexTok::LESS_EQ:	fprintf(_outFile,"LESS EQ ");	break;
	case sclLexTok::GREAT_EQ:	fprintf(_outFile,"GREATEQ ");	break;
	case sclLexTok::ADD:		fprintf(_outFile,"ADD     ");	break;
	case sclLexTok::SUB:		fprintf(_outFile,"SUB     ");	break;
	case sclLexTok::OR:			fprintf(_outFile,"OR      ");	break;
	case sclLexTok::TIMES:		fprintf(_outFile,"TIMES   ");	break;
	case sclLexTok::DIV:		fprintf(_outFile,"DIV     ");	break;
	case sclLexTok::MOD:		fprintf(_outFile,"MOD     ");	break;
	case sclLexTok::AND:		fprintf(_outFile,"AND     ");	break;
	case sclLexTok::LIT_FALSE:	fprintf(_outFile,"FALSE   ");	break;
	case sclLexTok::LIT_TRUE:	fprintf(_outFile,"TRUE    ");	break;
	} // END OF switch(_tok->m_sub)
	fputc('\t',_outFile);

	switch(_tok->m_tag)
	{
	case sclLexTok::IDENT:		
		fprintf(_outFile,"%s \n",(char*)(_tok->m_data));	
		break;
	case sclLexTok::STRING_LIT:	
		fprintf(_outFile,"%s \n",(char*)(_tok->m_data));	
		break;
	case sclLexTok::FLOAT_LIT:	
		fprintf(_outFile,"%f \n",*((float*)(_tok->m_data)));
		break;
	case sclLexTok::INT_LIT:	
		fprintf(_outFile,"%i \n",*((int*)(_tok->m_data)));		
		break;
	case sclLexTok::BOOL_LIT:	
		fprintf(_outFile,"-\n");		
		break;
	default:	
		fprintf(_outFile,"-\n");	
		break;
	} // END OF switch(_tok->m_tag)
}

//============================================================================
//  Prints a Symbol from the Symbol Table
//============================================================================
void PrintSymbol( sclSymbol* _symbol, FILE* _outFile )
{
	/*----------------------------------
		Print Out Ident
	----------------------------------*/
	fprintf(_outFile,"%s \t",_symbol->m_ident);

	/*----------------------------------
		Print out Symbol Type
	----------------------------------*/
	switch(_symbol->m_type)
	{
	case sclSymbol::NULL_TYPE:	fprintf(_outFile,"null  \t");	break;
	case sclSymbol::BOOL_TYPE:	fprintf(_outFile,"bool  \t");	break;
	case sclSymbol::INT_TYPE:	fprintf(_outFile,"int   \t");	break;
	case sclSymbol::FLOAT_TYPE:	fprintf(_outFile,"float \t");	break;
	case sclSymbol::STRING_TYPE:fprintf(_outFile,"string\t");	break;
	default:	
		fprintf(_outFile,"unkown\t");
		break;
	}; // END OF switch(_symbol->m_type)

	/*----------------------------------
		Print Out Data
	----------------------------------*/
	switch(_symbol->m_type)
	{
	case sclSymbol::NULL_TYPE:	
		fprintf(_outFile,"%p  \n",_symbol->m_data);	
		break;

	case sclSymbol::BOOL_TYPE:	
		if(*((bool*)_symbol->m_data))
		{
			fprintf(_outFile,"TRUE \n");
		}
		else
		{
			fprintf(_outFile,"FALSE \n");
		}break;

	case sclSymbol::INT_TYPE:
		fprintf(_outFile,"%i \n",*((int*)(_symbol->m_data)));		
		break;

	case sclSymbol::FLOAT_TYPE:	
		fprintf(_outFile,"%f \n", (float*)(_symbol->m_data));	
		break;

	case sclSymbol::STRING_TYPE:
		fprintf(_outFile,"%s \n", _symbol->m_data);
		break;

	default:	
		fprintf(_outFile,"UNKOWN!");
		break;
	}; // END OF switch(_symbol->m_type)
}