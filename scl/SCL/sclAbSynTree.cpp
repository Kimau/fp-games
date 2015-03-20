/*----------------------------------
	 Includes
   STDIO - Used for sprintf
   PRINTERS - Prints Out
 ----------------------------------*/
#include <stdio.h>
#include "sclAbSynTree.h"

//============================================================================
//  Constructor
//============================================================================
sclAbSynTree::sclAbSynTree()
{
	m_symTab = 0;
	m_expBlock = 0;
	m_print = ErrorPrinter::Instance();
}

//============================================================================
//  Deconstructor
//============================================================================
sclAbSynTree::~sclAbSynTree()
{
	/*----------------------------------
		 TODO :: Free AST
	 ----------------------------------*/
}

//============================================================================
//  Generate Abstract Syntax Tree
//============================================================================
void sclAbSynTree::Generate(sclLexTok* _token,		// *IN* Head of Token
							sclSymTable* _symTab)	// *IN* Symbol Table
{
	/*----------------------------------
		 Check Okay for Gen
	 ----------------------------------*/
	if(_symTab == 0)
	{
		m_print->Print(5,"NULL Symbol Table");
	}
	if (m_expBlock != 0)
	{
		m_print->Print(5,"Attempted to Generate Twice");
	}

	/*----------------------------------
		Setup Default Params 
	 ----------------------------------*/
	m_symTab = _symTab;

	/*----------------------------------
		 Generate AST
	 ----------------------------------*/
	sclLexTok* _temp = _token;
	m_expBlock = GenExpression(_temp,0);
	/*----------------------------------
		 Generate Reverse Polish
	 ----------------------------------*/
	RevPolish(_token);
}

//============================================================================
//  Create a Blank Expression
//============================================================================
sclExp* sclAbSynTree::CreateExp()
{
	/*----------------------------------
	Create New Expression
	----------------------------------*/
	sclExp* _exp = new sclExp;
	_exp->m_type = sclExp::NULL_TYPE;
	_exp->m_oper = sclExp::NULL_SUB;
	_exp->m_data = 0;
	return _exp;
}

//============================================================================
//  Generate Reverse Polish Notation
//	Users the Shunting Yard Algorithm
//============================================================================
void sclAbSynTree::RevPolish(sclLexTok* _token) // *IN* Start of Tokens
{
	rPolStack _stack;
	/*----------------------------------
		 Read Tokens until END
	 ----------------------------------*/
	while((_token != 0) && (_token->m_tag != sclLexTok::END))
	{
		switch(_token->m_tag)
		{
			/*----------------------------------
				 If Token is a Factor push
				 onto output
			 ----------------------------------*/
		case sclLexTok::BOOL_LIT:
		case sclLexTok::FLOAT_LIT:
		case sclLexTok::INT_LIT:
		case sclLexTok::STRING_LIT:
		case sclLexTok::IDENT:
			m_polTokQ.push(_token);
			break;
			/*----------------------------------
				 If Token is a Operator push
				 onot stack
			 ----------------------------------*/
		case sclLexTok::ADDOP:
		case sclLexTok::MULOP:
		case sclLexTok::RELOP:
			{
				while((_stack.empty() == false) &&	// Stack Not Empty
					  (_stack.top()->m_tag <= _token->m_tag)) 
					                                // Token High Precedence
				{
					m_polTokQ.push(_stack.top());	// Put onto Output Queue
					_stack.pop();				// Remove from Stack
				}
				_stack.push(_token); // Put Operator onto the stack
			} 
			break;
			/*----------------------------------
			Figure it OUT
			----------------------------------*/
		case sclLexTok::NOTOP:
			{
				while((_stack.empty() == false) &&	// Stack Not Empty
					(_stack.top()->m_tag < _token->m_tag)) 
					// Token High Precedence
				{
					m_polTokQ.push(_stack.top());	// Put onto Output Queue
					_stack.pop();				// Remove from Stack
				}
				_stack.push(_token); // Put Operator onto the stack
			} 
			break;
			/*----------------------------------
				 Open Brackets
			 ----------------------------------*/
		case sclLexTok::LEFTBKT:
			{
				_stack.push(_token);
			}
			break;
			/*----------------------------------
				 Close Brackets
			 ----------------------------------*/
		case sclLexTok::RIGHTBKT:
			while((_stack.empty() == false) && // Stack Not Empty
				  (_stack.top()->m_tag != sclLexTok::LEFTBKT))
			{
				m_polTokQ.push(_stack.top());	// Put onto Output Queue
				_stack.pop();				// Remove from Stack
			}
			/*----------------------------------
				No Matching Bracket
			----------------------------------*/
			if(_stack.top()->m_tag != sclLexTok::LEFTBKT)
			{
				m_print->Print(5,"No Matching Bracket (");
			}
			_stack.pop();
			break;
		}
		/*----------------------------------
			 Get Next Token
		 ----------------------------------*/
		_token = _token->m_next;
	} // END OF while(_token->m_tag != sclLexTok::END)

	/*----------------------------------
		 Clear the Stack
	 ----------------------------------*/
	while(_stack.empty() == false)
	{
		sclLexTok* _topTok = _stack.top();		// Get Top
		_stack.pop();							// Pop Top
		m_polTokQ.push(_topTok);				// Add Token to Output

		/*----------------------------------
			 Mismatched Paren
		 ----------------------------------*/
		if(_topTok->m_tag == sclLexTok::LEFTBKT)
		{
			m_print->Print(5,"Mismatched Paren: expected )");
		}
	}
}

//============================================================================
//  Get Tokens from Rev Polish Queue
//============================================================================
sclLexTok* sclAbSynTree::PopPolish()
{
	/*----------------------------------
		 Check for Empty
	 ----------------------------------*/
	if (m_polTokQ.empty() == true)
	{
		return 0;
	}
	/*----------------------------------
		 Return Token (destructive)
	 ----------------------------------*/
	sclLexTok* _temp = m_polTokQ.front();
	m_polTokQ.pop();
	return _temp;
}

//============================================================================
//  Generate Expression
//============================================================================
sclExp* sclAbSynTree::GenExpression(sclLexTok*& _token, sclExp* _prevExp)
{
// 	/*----------------------------------
// 		Generate New Expression
// 	----------------------------------*/
// 	sclExp* _exp = CreateExp();
// 
// 	/*----------------------------------
// 		Switch Tag
// 	----------------------------------*/
// 	switch(_token->m_tag)
// 	{
// 		/*----------------------------------
// 			Bool Literal
// 		----------------------------------*/
// 	case sclLexTok::BOOL_LIT:
// 		{
// 			bool* _data = new bool;
// 			_exp->m_oper = sclExp::BOOL_LIT;
// 
// 			if(_token->m_sub == sclLexTok::LIT_TRUE)
// 			{
// 				*_data = true;
// 			}
// 			else
// 			{
// 				*_data = false;
// 			}
// 
// 			_exp->m_oper = sclExp::VALUE;
// 			_exp->m_data = _data;
// 
// 			_token = _token->m_next;		// Get Next Token
// 		}
// 		break;
// 		/*----------------------------------
// 			Float Literal
// 		----------------------------------*/
// 	case sclLexTok::FLOAT_LIT:
// 		{
// 			float* _data = new float;
// 			memcpy(_data, _token->m_data, sizeof(float));
// 			_exp->m_oper = sclExp::FLOAT_LIT;
// 
// 			_exp->m_oper = sclExp::VALUE;
// 			_exp->m_data = _data;
// 
// 			_token = _token->m_next;		// Get Next Token
// 		}
// 		break;
// 		/*----------------------------------
// 			Integer Literal
// 		----------------------------------*/
// 	case sclLexTok::INT_LIT:
// 		{
// 			bool* _data = new bool;
// 			memcpy(_data, _token->m_data, sizeof(bool));
// 			_exp->m_oper = sclExp::INT_LIT;
// 
// 			_exp->m_type = sclExp::VALUE;
// 			_exp->m_data = _data;
// 
// 			_token = _token->m_next;		// Get Next Token
// 		}
// 		break;
// 		/*----------------------------------
// 			String Literal
// 		----------------------------------*/
// 	case sclLexTok::STRING_LIT:
// 		{
// 			char* _data = new string;
// 			strcpy(_data, _token->m_data);
// 			_exp->m_oper = sclExp::STRING_LIT;
// 
// 			_exp->m_type = sclExp::VALUE;
// 			_exp->m_data = _data;
// 
// 			_token = _token->m_next;		// Get Next Token
// 		}
// 		break;
// 		/*----------------------------------
// 			Identifier
// 		----------------------------------*/
// 	case sclLexTok::IDENT:
// 		{
// 			char* _data = new string;
// 			strcpy(_data, (char*)(_token->m_data));
// 			_exp->m_oper = sclExp::SYMBOL;
// 
// 			_exp->m_type = sclExp::VALUE
// 			_exp->m_data = _data;
// 
// 			_token = _token->m_next;		// Get Next Token
// 		}
// 		break;
// 		/*----------------------------------
// 			Operator
// 		----------------------------------*/
// 	case sclLexTok::ADDOP:
// 	case sclLexTok::MULOP:
// 	case sclLexTok::RELOP:
// 	case sclLexTok::NOTOP:
// 		{
// 			_exp->m_type = sclExp::TREE;
// 			_exp->m_oper = sclExp::OPERATION;
// 			sclExp* _tree = new sclExp[3];
// 			_tree[0] = _prevExp;					// Get Exp 1
// 			_tree[1] = GenOper(_token);				// Get Operator
// 			_tree[2] = GenExpression(_token,_exp);	// Get Exp 2
// 
// 			_exp->m_data = _tree;
// 		} 
// 		break;
// 		/*----------------------------------
// 			Open Brackets
// 		----------------------------------*/
// 	case sclLexTok::LEFTBKT:
// 		{
// 
// 		}
// 		break;
// 		/*----------------------------------
// 			Close Brackets
// 		----------------------------------*/
// 	case sclLexTok::RIGHTBKT:
// 		{
// 
// 		}
// 		break;
// 		/*----------------------------------
// 			 End of Expressions
// 		 ----------------------------------*/
// 	case sclLexTok::END:
// 		break;
// 		/*----------------------------------
// 			 Something Gone Wrong
// 		 ----------------------------------*/
// 	default:
// 		break;
// 	}
	return 0;
}

//============================================================================
//  Generate Operator
//============================================================================
sclExp* sclAbSynTree::GenOper(sclLexTok* _token)
{
// 	/*----------------------------------
// 		Get Operator Type
// 	----------------------------------*/
// 	switch(_token->m_sub)
// 	{
// 	case sclLexTok::ADDOP:
// 		break;
// 	case sclLexTok::NULL_SUB:
// 		break;
// 	case sclLexTok::EQUAL:
// 		break;
// 	case sclLexTok::NOT_EQ:
// 		break;
// 	case sclLexTok::LESS:
// 		break;
// 	case sclLexTok::GREAT:
// 		break;
// 	case sclLexTok::LESS_EQ:
// 		break;
// 	case sclLexTok::GREAT_EQ:
// 		break;
// 	case sclLexTok::ADD:
// 		break;
// 	case sclLexTok::SUB:
// 		break;
// 	case sclLexTok::OR:
// 		break;
// 	case sclLexTok::TIMES:
// 		break;
// 	case sclLexTok::DIV:
// 		break;
// 	case sclLexTok::MOD:
// 		break;
// 	case sclLexTok::AND:
// 		break;
// 	}
	return 0;
}