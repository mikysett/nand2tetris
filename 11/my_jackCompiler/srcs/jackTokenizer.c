#include "jackCompiler.h"

static int			ft_is_symbol(char c)
{
	switch (c)
	{
	case '{':
	case '}':
	case '(':
	case ')':
	case '[':
	case ']':
	case '.':
	case ',':
	case ';':
	case '+':
	case '-':
	case '*':
	case '/':
	case '&':
	case '|':
	case '<':
	case '>':
	case '=':
	case '~':
		return (1);
	default:
		return (0);
	}
}

static t_keyword	ft_is_keyword(char *str)
{
	if (!strcmp(str, "class"))
		return (CLASS);
	if (!strcmp(str, "constructor"))
		return (CONSTRUCTOR);
	if (!strcmp(str, "method"))
		return (METHOD);
	if (!strcmp(str, "function"))
		return (FUNCTION);
	if (!strcmp(str, "int"))
		return (INT);
	if (!strcmp(str, "boolean"))
		return (BOOLEAN);
	if (!strcmp(str, "char"))
		return (CHAR);
	if (!strcmp(str, "void"))
		return (VOID);
	if (!strcmp(str, "var"))
		return (VAR);
	if (!strcmp(str, "static"))
		return (STATIC);
	if (!strcmp(str, "field"))
		return (FIELD);
	if (!strcmp(str, "let"))
		return (LET);
	if (!strcmp(str, "do"))
		return (DO);
	if (!strcmp(str, "if"))
		return (IF);
	if (!strcmp(str, "else"))
		return (ELSE);
	if (!strcmp(str, "while"))
		return (WHILE);
	if (!strcmp(str, "return"))
		return (RETURN);
	if (!strcmp(str, "true"))
		return (TRUE);
	if (!strcmp(str, "false"))
		return (FALSE);
	if (!strcmp(str, "null"))
		return (KW_NULL);
	if (!strcmp(str, "this"))
		return (THIS);
	return (0);
}

static t_token	*add_token(void)
{
	t_token *new_token;

	new_token = calloc(1, sizeof(t_token));
	return (new_token);
}

char				*inc_token_size(char *actualToken, int new_size)
{
	char *biggerT;

	biggerT = malloc(sizeof(char) * (new_size + 1));
	if (!biggerT)
		return (0);
	strcpy(biggerT, actualToken);
	free(actualToken);
	return (biggerT);
}

static void			ft_skip_comments(FILE *srcFile, t_commentType commType)
{
	char	buff;
	char	last_buff;

	last_buff = '\0';
	if (commType == INLINE)
	{
		while (fread(&buff, 1, 1, srcFile))
			if (buff == '\n')
				break;
		fseek(srcFile, -1, SEEK_CUR);
	}
	else
	{
		while (fread(&buff, 1, 1, srcFile))
		{
			if (last_buff == '*' && buff == '/')
				break;
			last_buff = buff;
		}
	}
}

static char			look_ahead(FILE *srcFile)
{
	char	buf;
	
	fread(&buf, 1, 1, srcFile);
	fseek(srcFile, -1, SEEK_CUR);
	return (buf);
}

static int			get_nextToken(FILE *srcFile,
	char *next_token, size_t *max_token_len)
{
	char	buf;
	char	next_buf;
	size_t	i;


	i = 0;
	*next_token = '\0';
	while (fread(&buf, 1, 1, srcFile))
	{
		if (*next_token == '"')
		{
			if (i > 1 && next_token[i - 1] == '"')
			{
				fseek(srcFile, -1, SEEK_CUR);
				break;
			}
		}
		else
		{
			if (buf == '/')
			{
				next_buf = look_ahead(srcFile);
				if (next_buf == '/' || next_buf == '*')
				{
					if (next_buf == '/')
						ft_skip_comments(srcFile, INLINE);
					else
						ft_skip_comments(srcFile, MULTILINE);
					continue;
				}
			}
			if (isspace(buf))
			{
				if (!i)
					continue;
				break;
			}
			if (ft_is_symbol(buf))
			{
				if (i == 0)
					next_token[i++] = buf;
				else
					fseek(srcFile, -1, SEEK_CUR);
				break;
			}
		}
		next_token[i] = buf;
		i++;
		if (i == *max_token_len)
		{
			next_token[i] = '\0';
			*max_token_len *= 2;
			inc_token_size(next_token, *max_token_len);
		}
	}
	if (!i)
		return (0);
	next_token[i] = '\0';
	return (1);
}

char				*ft_copy_stringconst(char *str)
{
	char	*stringval;
	char	*start_stringval;

	stringval = malloc(sizeof(char) * (strlen(str) - 1));
	start_stringval = stringval;
	if (!stringval)
		return (0);
	str++;
	while (*str != '"')
		*stringval++ = *str++;
	*stringval = '\0';
	return (start_stringval);
}

t_list			**jackTokenizer(FILE *src)
{
	char		*nextToken;
	size_t		max_token_len;
	t_token 	*new_token;
	t_list		**tokens;

	tokens = calloc(1, sizeof(t_list *));
	if (!tokens)
		return (0);
	nextToken = calloc(33, 1);
	if (!nextToken)
		return (0);
	max_token_len = 32;
	while (get_nextToken(src, nextToken, &max_token_len))
	{
		new_token = add_token();
		if (ft_is_symbol(nextToken[0]))
		{
			new_token->tokenType = SYMBOL;
			new_token->symbol = nextToken[0];
		}
		else if (ft_is_keyword(nextToken))
		{
			new_token->tokenType = KEYWORD;
			new_token->keyword = ft_is_keyword(nextToken);
		}
		else if (nextToken[0] == '"')
		{
			new_token->tokenType = STRING_CONST;
			new_token->stringval = ft_copy_stringconst(nextToken);
		}
		else if (isdigit(nextToken[0]))
		{
			new_token->tokenType = INT_CONST;
			new_token->intval = atoi(nextToken);
		}
		else // IDENTIFIER
		{
			new_token->tokenType = IDENTIFIER;
			new_token->identifier = strdup(nextToken);
		}
		ft_lstadd_back(tokens, ft_lstnew(new_token));
		DEBUG_CODE(printf("token: %s\n", nextToken);)
	}
	free(nextToken);
	return (tokens);
}