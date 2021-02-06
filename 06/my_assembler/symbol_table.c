#include "assembler.h"

static void		add_sbl(t_sbl **lst, t_sbl *new)
{
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	while ((*lst)->next)
		(lst) = &((*lst)->next);
	(*lst)->next = new;
}

static t_sbl	*new_sbl(char *symbol, int address)
{
	t_sbl	*new_sbl;

	new_sbl = malloc(sizeof(t_sbl));
	new_sbl->symbol = strdup(symbol);
	new_sbl->address = address;
	new_sbl->next = 0;
	return (new_sbl);
}

static void		ft_init_variables(t_sbl **symbols)
{
	add_sbl(symbols, new_sbl("R0", 0));
	add_sbl(symbols, new_sbl("R1", 1));
	add_sbl(symbols, new_sbl("R2", 2));
	add_sbl(symbols, new_sbl("R3", 3));
	add_sbl(symbols, new_sbl("R4", 4));
	add_sbl(symbols, new_sbl("R5", 5));
	add_sbl(symbols, new_sbl("R6", 6));
	add_sbl(symbols, new_sbl("R7", 7));
	add_sbl(symbols, new_sbl("R8", 8));
	add_sbl(symbols, new_sbl("R9", 9));
	add_sbl(symbols, new_sbl("R10", 10));
	add_sbl(symbols, new_sbl("R11", 11));
	add_sbl(symbols, new_sbl("R12", 12));
	add_sbl(symbols, new_sbl("R13", 13));
	add_sbl(symbols, new_sbl("R14", 14));
	add_sbl(symbols, new_sbl("R15", 15));
	add_sbl(symbols, new_sbl("SP", 0));
	add_sbl(symbols, new_sbl("LCL", 1));
	add_sbl(symbols, new_sbl("ARG", 2));
	add_sbl(symbols, new_sbl("THIS", 3));
	add_sbl(symbols, new_sbl("THAT", 4));
	add_sbl(symbols, new_sbl("SCREEN", 16384));
	add_sbl(symbols, new_sbl("KBD", 24576));
}

int				ft_get_address(t_sbl **symbols, char *sbl_name)
{
	t_sbl	*sbl;

	sbl = *symbols;
	while (sbl)
	{
		if (!strcmp(sbl->symbol, sbl_name))
			return (sbl->address);
		sbl = sbl->next;
	}
	return (-1);
}

static void		ft_set_labels(t_cmd **commands, t_sbl **symbols)
{
	int		i;
	int		address;
	t_cmd	*cmd;

	i = 0;
	cmd = *commands;
	while (cmd)
	{
		if (cmd->type == 'L')
		{
			address = ft_get_address(symbols, cmd->symbol);
			if (address < 0)
				add_sbl(symbols, new_sbl(cmd->symbol, i));
		}
		else
			i++;
		cmd = cmd->next;
	}
}

static void		ft_set_variables(t_cmd **commands, t_sbl **symbols)
{
	int	i;
	int address;
	t_cmd	*cmd;

	i = 16;
	cmd = *commands;
	while (cmd)
	{
		if (cmd->type == 'A' && (cmd->symbol[0] < '0' || cmd->symbol[0] > '9'))
		{
			address = ft_get_address(symbols, cmd->symbol);
			if (address < 0)
			{
				add_sbl(symbols, new_sbl(cmd->symbol, i));
				i++;
			}
		}
		cmd = cmd->next;
	}
}

DEBUG_CODE(
static void		ft_print_sbls(t_sbl **sbl)
{
	t_sbl	*curr;

	printf("\n---- SYMBOL TABLE\n");
	curr = *sbl;
	while(curr)
	{
		printf("%6d | %16s |\n", curr->address, curr->symbol);
		curr = curr->next;
	}
	printf("\n");
})

void			symbol_table(t_cmd **commands, t_sbl **symbols)
{

	ft_init_variables(symbols);
	ft_set_labels(commands, symbols);
	ft_set_variables(commands, symbols);
	
	DEBUG_CODE(ft_print_sbls(symbols);)
}