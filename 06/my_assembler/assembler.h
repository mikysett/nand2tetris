#ifndef ASSEMBLER_H
# define ASSEMBLER_H

# define DEBUG_MODE		0
# if DEBUG_MODE == 1
#  define DEBUG_CODE(x)	x
# else
#  define DEBUG_CODE(x)
# endif

# define MAX_LINE_SIZE	160
# define ERR_NO_ARG		0
# define ERR_OPEN_FAIL	1
# define ERR_MAX_L_SIZE	3
# define ERR_WRONG_EXT	4
# define ERR_DST_FILE	5

# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>

typedef struct s_cmd
{
	char			type;		// 'A', 'C' or 'L'

	// For 'A' and 'L' instructions
	char			*symbol;	// xxx of @xxx or (xxx)

	// For 'C' instructions
	char			*dest;		// mnemonic dest of the operation
	char			*comp;		// mnemonic comp of the operation
	char			*jmp;		// mnemonic jmp of the operation

	struct s_cmd	*next;
}				t_cmd;

typedef struct s_sbl
{
	char			*symbol;
	int				address;
	struct s_sbl	*next;
}				t_sbl;



int			parser(FILE *srcFile, t_cmd **commands);
void		symbol_table(t_cmd **commands, t_sbl **symbols);
int			ft_get_address(t_sbl **symbols, char *sbl_name);
void		code(FILE* dst_f, t_cmd **commands, t_sbl **symbols);
void		ft_free_split(char **split_result);

#endif