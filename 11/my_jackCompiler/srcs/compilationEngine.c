#include "jackCompiler.h"

static t_list	*compileTerm(FILE* dst, t_list *token);
static t_list	*compileStatements(FILE* dst, t_list *token);
static t_list	*compileExpression(FILE* dst, t_list *token);

static t_list	*compilationError(FILE *dst, char *location)
{
	fprintf(dst, "<err>%s: COMPILATION ERROR!</err>\n", location);
	return (0);
}

static int			ft_is_op(t_token *token)
{
	if (token->tokenType == SYMBOL)
	{
		switch (token->symbol)
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '&':
		case '|':
		case '<':
		case '>':
		case '=':
			return (1);
		}
	}
	return (0);
}

static int			ft_is_unary_op(t_token *token)
{
	if (token->tokenType == SYMBOL)
	{
		switch (token->symbol)
		{
		case '-':
		case '~':
			return (1);
		}
	}
	return (0);
}

static int			ft_print_symbol(FILE *dst, t_token *token, char symbol)
{
	if (token
		&& ((token->tokenType == SYMBOL && token->symbol == symbol)
		|| (token->tokenType == SYMBOL && symbol == 0)))
	{
		if (dst)
		{
			if (token->symbol == '<')
				fprintf(dst, "<symbol> &lt; </symbol>\n");
			else if (token->symbol == '>')
				fprintf(dst, "<symbol> &gt; </symbol>\n");
			else if (token->symbol == '&')
				fprintf(dst, "<symbol> &amp; </symbol>\n");
			else
				fprintf(dst, "<symbol> %c </symbol>\n", token->symbol);
		}
		return (1);
	}
	return (0);
}

static t_list	*compileVarDec(FILE* dst, t_list *token)
{
	fprintf(dst, "<varDec>\n");
	while (!ft_print_symbol(dst, token->content, ';'))
	{
		token = compilationEngine(dst, token);
		if (ft_print_symbol(dst, token->content, ','))
			token = token->next;
	}
	fprintf(dst, "</varDec>\n");
	return (token->next);
}

static t_list	*compileParameterList(FILE* dst, t_list *token)
{
	fprintf(dst, "<parameterList>\n");
	if (!ft_print_symbol(0, (t_token *)token->content, ')'))
	{
		token = compilationEngine(dst, token);
		while (ft_print_symbol(dst, token->content, ','))
		{
			token = token->next;
			token = compilationEngine(dst, token);
		}
	}
	fprintf(dst, "</parameterList>\n");
	return (token);
}


static t_list	*compileExpressionList(FILE* dst, t_list *token)
{
	fprintf(dst, "<expressionList>\n");
	if (((t_token *)token->content)->symbol != ')')
	{
		token = compileExpression(dst, token);
		while (ft_print_symbol(dst, token->content, ','))
		{
			token = token->next;
			token = compileExpression(dst, token);
		}
	}
	fprintf(dst, "</expressionList>\n");
	return (token);
}

static t_list	*compileExpression(FILE* dst, t_list *token)
{
	fprintf(dst, "<expression>\n");
	token = compileTerm(dst, token);
	if (ft_is_op(token->content))
	{
		ft_print_symbol(dst, token->content, 0);
		token = token->next;
		token = compileTerm(dst, token);
	}
	fprintf(dst, "</expression>\n");
	return (token);
}

static t_list	*compileTerm(FILE* dst, t_list *token)
{
	fprintf(dst, "<term>\n");
	if (ft_is_unary_op(token->content))
	{
		fprintf(dst, "<symbol> %c </symbol>\n", ((t_token *)token->content)->symbol);
		token = compileTerm(dst, token->next);
	}
	if (((t_token *)token->content)->tokenType == IDENTIFIER)
	{
		// Subroutine call
		if (((t_token *)token->next->content)->symbol == '.'
			|| ((t_token *)token->next->content)->symbol == '(')
		{
			token = compilationEngine(dst, token);
			if (!ft_print_symbol(dst, token->content, '('))
				return (compilationError(dst, "compileTerm"));
			token = token->next;
			token = compileExpressionList(dst, token);
			if (!ft_print_symbol(dst, token->content, ')'))
				return (compilationError(dst, "compileTerm"));
			token = token->next;
		}
		// Array entry
		else if (ft_print_symbol(dst, token->content, '['))
			token = compileExpression(dst, token);
		else
			token = compilationEngine(dst, token);
	}
	else if (!ft_is_op(token->content))
	{
		if (ft_print_symbol(dst, token->content, '('))
		{
			token = token->next;
			token = compileExpression(dst, token);
			if (!ft_print_symbol(dst, token->content, ')'))
				return(compilationError(dst, "compileTerm"));
			token = token->next;
		}
		else
			token = compilationEngine(dst, token);
	}
	fprintf(dst, "</term>\n");
	if (ft_is_op(token->content))
	{
		ft_print_symbol(dst, token->content, 0);
		token = token->next;
		token = compileTerm(dst, token);
	}
	return (token);
}

static t_list	*compileLet(FILE* dst, t_list *token)
{
	fprintf(dst, "<letStatement>\n");
	token = compilationEngine(dst, token);
	if (!ft_print_symbol(dst, token->content, '='))
		return(compilationError(dst, "compileLet"));
	token = token->next;
	token = compileExpression(dst, token);
	if (!ft_print_symbol(dst, token->content, ';'))
		return(compilationError(dst, "compileLet"));
	fprintf(dst, "</letStatement>\n");
	return (token->next);
}

static t_list	*compileIf(FILE* dst, t_list *token)
{
	fprintf(dst, "<ifStatement>\n");
	token = compilationEngine(dst, token);
	if (!ft_print_symbol(dst, token->content, '('))
		return(compilationError(dst, "compileIf"));
	token = token->next;
	token = compileExpression(dst, token);
	if (!ft_print_symbol(dst, token->content, ')'))
		return(compilationError(dst, "compileIf"));
	token = token->next;
	if (!ft_print_symbol(dst, token->content, '{'))
		return(compilationError(dst, "compileIf"));
	token = token->next;
	token = compileStatements(dst, token);
	if (!ft_print_symbol(dst, token->content, '}'))
		return(compilationError(dst, "compileIf"));
	token = token->next;
	if (token && ((t_token *)token->content)->tokenType == KEYWORD
		&& ((t_token *)token->content)->keyword == ELSE)
	{
		fprintf(dst, "<keyword> else </keyword>\n");
		token = token->next;
		if (!ft_print_symbol(dst, token->content, '{'))
			return(compilationError(dst, "compileIf"));
		token = token->next;
		token = compileStatements(dst, token);
		if (!ft_print_symbol(dst, token->content, '}'))
			return(compilationError(dst, "compileIf"));
		token = token->next;
	}
	fprintf(dst, "</ifStatement>\n");
	return (token);
}

static t_list	*compileWhile(FILE* dst, t_list *token)
{
	fprintf(dst, "<whileStatement>\n");
	token = compilationEngine(dst, token);
	if (!ft_print_symbol(dst, token->content, '('))
			return(compilationError(dst, "compileWhile"));
		token = token->next;
		token = compileExpression(dst, token);
		if (!ft_print_symbol(dst, token->content, ')'))
			return(compilationError(dst, "compileWhile"));
	token = token->next;
	if (!ft_print_symbol(dst, token->content, '{'))
			return(compilationError(dst, "compileWhile"));
		token = token->next;
		token = compileStatements(dst, token);
		if (!ft_print_symbol(dst, token->content, '}'))
			return(compilationError(dst, "compileWhile"));
	fprintf(dst, "</whileStatement>\n");
	return (token->next);
}

static t_list	*compileDo(FILE* dst, t_list *token)
{
	fprintf(dst, "<doStatement>\n");
	token = compilationEngine(dst, token);
	if (!ft_print_symbol(dst, token->content, '('))
		return(compilationError(dst, "compileDo"));
	token = token->next;
	token = compileExpressionList(dst, token);
	if (!ft_print_symbol(dst, token->content, ')'))
		return(compilationError(dst, "compileDo"));
	token = token->next;
	if (!ft_print_symbol(dst, token->content, ';'))
		return(compilationError(dst, "compileDo"));
	fprintf(dst, "</doStatement>\n");
	return (token->next);
}

static t_list	*compileReturn(FILE* dst, t_list *token)
{
	fprintf(dst, "<returnStatement>\n");
	fprintf(dst, "<keyword> return </keyword>\n");
	token = token->next;
	if (!ft_print_symbol(dst, token->content, ';'))
	{
		token = compileExpression(dst, token);
		if (!ft_print_symbol(dst, token->content, ';'))
			return(compilationError(dst, "compileReturn"));
	}
	fprintf(dst, "</returnStatement>\n");
	return (token->next);
}

static t_list	*compileStatements(FILE* dst, t_list *token)
{

	fprintf(dst, "<statements>\n");
	while (token
		&& (((t_token *)token->content)->tokenType != SYMBOL
			|| ((t_token *)token->content)->symbol != '}'))
	{
		if (((t_token *)token->content)->keyword == LET)
			token = compileLet(dst, token);
		else if (((t_token *)token->content)->keyword == IF)
			token = compileIf(dst, token);
		else if (((t_token *)token->content)->keyword == WHILE)
			token = compileWhile(dst, token);
		else if (((t_token *)token->content)->keyword == DO)
			token = compileDo(dst, token);
		else if (((t_token *)token->content)->keyword == RETURN)
			token = compileReturn(dst, token);
		else
		{
			compilationEngine(dst, token);
			token = token->next;
		}
	}
	fprintf(dst, "</statements>\n");
	return (token);
}

static t_list	*compileSubroutine(FILE* dst, t_list *token)
{
	fprintf(dst, "<subroutineDec>\n");
	token = compilationEngine(dst, token);
	if (!ft_print_symbol(dst, token->content, '('))
		return(compilationError(dst, "compileSubroutine"));
	token = token->next;
	token = compileParameterList(dst, token);
	if (!ft_print_symbol(dst, token->content, ')'))
		return(compilationError(dst, "compileSubroutine"));
	token = token->next;
	fprintf(dst, "<subroutineBody>\n");
	if (!ft_print_symbol(dst, token->content, '{'))
		return(compilationError(dst, "compileSubroutine"));
	token = token->next;
	while (token && ((t_token *)token->content)->tokenType == KEYWORD
		&& ((t_token *)token->content)->keyword == VAR)
		token = compileVarDec(dst, token);
	token = compileStatements(dst, token);
	if (!ft_print_symbol(dst, token->content, '}'))
		return(compilationError(dst, "compileSubroutine"));
	fprintf(dst, "</subroutineBody>\n");
	fprintf(dst, "</subroutineDec>\n");
	return (token->next);
}

static t_list	*compileClassVarDec(FILE* dst, t_list *token)
{
	fprintf(dst, "<classVarDec>\n");
	while (!ft_print_symbol(dst, token->content, ';'))
	{
		token = compilationEngine(dst, token);
		if (ft_print_symbol(dst, token->content, ','))
			token = token->next;
	}
	fprintf(dst, "</classVarDec>\n");
	return (token->next);
}

static t_list	*compileClass(FILE* dst, t_list *token)
{
	fprintf(dst, "<class>\n");
	fprintf(dst, "<keyword> class </keyword>\n");
	token = compilationEngine(dst, token->next);
	if (!ft_print_symbol(dst, token->content, '{'))
		return(compilationError(dst, "compileClass"));
	token = token->next;
	while (token && ((t_token *)token->content)->tokenType == KEYWORD
		&& (((t_token *)token->content)->keyword == STATIC
		|| ((t_token *)token->content)->keyword == FIELD))
		token = compileClassVarDec(dst, token);
	while (token && ((t_token *)token->content)->tokenType == KEYWORD
		&& (((t_token *)token->content)->keyword == CONSTRUCTOR
		|| ((t_token *)token->content)->keyword == FUNCTION
		|| ((t_token *)token->content)->keyword == METHOD))
	{
		token = compileSubroutine(dst, token);
	}
	if (!ft_print_symbol(dst, token->content, '}'))
		return(compilationError(dst, "compileClass"));
	fprintf(dst, "</class>\n");
	return (token);
}

t_list 		*compilationEngine(FILE* dst, t_list *token)
{
	if (!token)
		return (0);
	if (((t_token *)token->content)->tokenType == KEYWORD)
	{
		switch (((t_token *)token->content)->keyword)
		{
		case CLASS:
			compileClass(dst, token);
			return (0);
		case FUNCTION:
			fprintf(dst, "<keyword> function </keyword>\n");
			break;
		case STATIC:
			fprintf(dst, "<keyword> static </keyword>\n");
			break;
		case FIELD:
			fprintf(dst, "<keyword> field </keyword>\n");
			break;
		case INT:
			fprintf(dst, "<keyword> int </keyword>\n");
			break;
		case BOOLEAN:
			fprintf(dst, "<keyword> boolean </keyword>\n");
			break;
		case CHAR:
			fprintf(dst, "<keyword> char </keyword>\n");
			break;
		case VOID:
			fprintf(dst, "<keyword> void </keyword>\n");
			break;
		case VAR:
			fprintf(dst, "<keyword> var </keyword>\n");
			break;
		case LET:
			fprintf(dst, "<keyword> let </keyword>\n");
			break;
		case TRUE:
			fprintf(dst, "<keyword> true </keyword>\n");
			break;
		case FALSE:
			fprintf(dst, "<keyword> false </keyword>\n");
			break;
		case KW_NULL:
			fprintf(dst, "<keyword> null </keyword>\n");
			break;
		case IF:
			fprintf(dst, "<keyword> if </keyword>\n");
			break;
		case ELSE:
			fprintf(dst, "<keyword> else </keyword>\n");
			break;
		case WHILE:
			fprintf(dst, "<keyword> while </keyword>\n");
			break;
		case DO:
			fprintf(dst, "<keyword> do </keyword>\n");
			break;
		case RETURN:
			fprintf(dst, "<keyword> return </keyword>\n");
			break;
		case CONSTRUCTOR:
			fprintf(dst, "<keyword> constructor </keyword>\n");
			break;
		case THIS:
			fprintf(dst, "<keyword> this </keyword>\n");
			break;
		case METHOD:
			fprintf(dst, "<keyword> method </keyword>\n");
			break;
		default:
			fprintf(dst, "<err>ERROR: KEYWORD NOT FOUND</err>\n");
			break;
		}
	}
	else if (((t_token *)token->content)->tokenType == IDENTIFIER)
		fprintf(dst, "<identifier> %s </identifier>\n",
			((t_token *)token->content)->identifier);
	else if (((t_token *)token->content)->tokenType == STRING_CONST)
		fprintf(dst, "<stringConstant> %s </stringConstant>\n",
			((t_token *)token->content)->stringval);
	else if (((t_token *)token->content)->tokenType == INT_CONST)
		fprintf(dst, "<integerConstant> %d </integerConstant>\n",
			((t_token *)token->content)->intval);
	else if (((t_token *)token->content)->tokenType == SYMBOL)
	{
		switch (((t_token *)token->content)->symbol)
		{
		case '{':
		case '}':
		case '(':
		case ')':
		case ',':
		case ';':
		case ']':
		case '*':
		case '/':
		case '+':
		case '-':
		case '&':
		case '|':
		case '<':
		case '>':
		case '=':
			return (token);
		case '[':
			ft_print_symbol(dst, token->content, '[');
			token = compileExpression(dst, token->next);
			if (!ft_print_symbol(dst, token->content, ']'))
				return (compilationError(dst, "compialtionEngine"));
			return (token->next);
		default:
			fprintf(dst, "<symbol> %c </symbol>\n",
				((t_token *)token->content)->symbol);
		}
	}
	return (compilationEngine(dst, token->next));
}