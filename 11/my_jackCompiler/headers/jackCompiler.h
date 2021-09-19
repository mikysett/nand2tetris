#ifndef JACKANALYZER_H
# define JACKANALYZER_H

# ifdef DEBUG_MODE
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

# include "libft.h"

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

typedef struct	s_token
{
	t_tokenType			tokenType;
	t_keyword			keyword;
	char				symbol;
	char				*identifier;
	int					intval;
	char				*stringval;
}				t_token;

t_list			**jackTokenizer(FILE *src);
t_list		 	*compilationEngine(FILE* dst, t_list *token);

// FILES_UTIL
t_list			**set_sources(char *path, char *ext_in, char *ext_out);

// MEMORY_UTIL
void			ft_free_sources(t_list **src_files);
void			ft_free_tokens(t_list **tokens);

// ERROR_UTIL
int				print_error(t_err err);

#endif