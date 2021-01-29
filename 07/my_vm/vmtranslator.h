#ifndef ASSEMBLER_H
# define ASSEMBLER_H

# define DEBUG_MODE		0
# if DEBUG_MODE == 1
#  define DEBUG_CODE(x)	x
# else
#  define DEBUG_CODE(x)
# endif

# define MAX_LINE_SIZE	160
# define POS_STATIC_VAR	16
# define POS_STACK		256
# define POS_HEAP		2048

# define ERR_NO_ARG		0
# define ERR_OPEN_FAIL	1
# define ERR_MAX_L_SIZE	3
# define ERR_WRONG_EXT	4
# define ERR_DST_FILE	5

# include <libgen.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>

typedef enum	e_type
{
	// Arithmetics and logical commands
	add, sub, neg, eq, gt, lt, and, or, not,
	// Memory access commands
	push, pop,
	// Program flow commands
	c_label, c_goto, c_if_goto,
	// Function calling commands
	c_function, c_call, c_return
}				t_type;

typedef struct s_cmd
{
	t_type			type;

	char			*arg1;
	char			*arg2;

	struct s_cmd	*next;
}				t_cmd;

typedef struct s_sbl
{
	char			*symbol;
	int				address;
	struct s_sbl	*next;
}				t_sbl;



int			errorExit(int err);
int			parser(FILE *srcFile, t_cmd **commands);
char		**ft_split(char const *s, char c);
void		code(FILE* dst_f, t_cmd **commands, char *b_name);


#endif