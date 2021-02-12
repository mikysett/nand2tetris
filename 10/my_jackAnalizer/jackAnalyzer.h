#ifndef JACKANALYZER_H
# define JACKANALYZER_H

# define DEBUG_MODE		1
# if DEBUG_MODE == 1
#  define DEBUG_CODE(x)	x
# else
#  define DEBUG_CODE(x)
# endif

# include <libgen.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <dirent.h>

typedef enum	e_err
{
	ERR_NO_ARG = 1,
	ERR_OPEN_FAIL,
	ERR_OPEN_DIR_FAIL,
	ERR_MAX_L_SIZE,
	ERR_WRONG_EXT,
	ERR_DST_FILE
}				t_err;

typedef struct	s_srcFile
{
	char				*src_name;
	char				*dst_name;
	struct s_srcFile	*next;
}				t_srcFile;

typedef enum	e_commentTtpe
{
	INLINE,
	MULTILINE
}				t_commentType;

typedef enum	e_tokenType
{
	KEYWORD = 1,
	SYMBOL,
	IDENTIFIER,
	INT_CONST,
	STRING_CONST
}				t_tokenType;

typedef enum	e_keyword
{
	CLASS = 1,
	METHOD,
	FUNCTION,
	CONSTRUCTOR,
	INT,
	BOOLEAN,
	CHAR,
	VOID,
	VAR,
	STATIC,
	FIELD,
	LET,
	DO,
	IF,
	ELSE,
	WHILE,
	RETURN,
	TRUE,
	FALSE,
	KW_NULL,
	THIS
}				t_keyword;

typedef struct	s_token_lst
{
	t_tokenType			tokenType;
	t_keyword			keyword;
	char				symbol;
	char				*identifier;
	int					intval;
	char				*stringval;
	struct s_token_lst	*next;
}				t_token_lst;

int				errorExit(t_err err);
char			**ft_split(char const *s, char c);
void			ft_free_split(char **split_result);
t_token_lst		**jackTokenizer(FILE *src);
t_token_lst 	*compilationEngine(FILE* dst, t_token_lst *token);

#endif