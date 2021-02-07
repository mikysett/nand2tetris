#include "assembler.h"

static int	set_digit(char *res, int num)
{
	int position;

	position = 0;
	if (num >= 2)
		position = set_digit(res, num / 2);
	res[position] = '0' + (num % 2);
	return (position + 1);
}

static int	set_nb_digits(int num)
{
	int i;

	i = 1;
	while (num >= 2)
	{
		num /= 2;
		i++;
	}
	return (i);
}

static char	*ft_dec_to_bin(int nbr)
{
	int		nb_digits;
	char	*final_nbr;

	nb_digits = set_nb_digits(nbr);
	final_nbr = malloc(sizeof(char) * (nb_digits + 1));
	if (!final_nbr)
		return (0);
	final_nbr[nb_digits] = '\0';
	set_digit(final_nbr, nbr);
	return (final_nbr);
}

static void	ft_print_bytes(FILE* dst_f, int nb, int size)
{
	char	*nb_binary;
	int		nb_size;

	nb_binary = ft_dec_to_bin(nb);
	nb_size = strlen(nb_binary);
	while (size > nb_size)
	{
		fprintf(dst_f, "0");
		size--;
	}
	fprintf(dst_f, "%s", nb_binary);
	free(nb_binary);
}

static void	ft_code_a(FILE* dst_f, t_cmd *cmd, t_sbl **symbols)
{
	fprintf(dst_f, "0");
	if (cmd->symbol[0] < '0' || cmd->symbol[0] > '9')
		ft_print_bytes(dst_f, ft_get_address(symbols, cmd->symbol), 15);
	else
		ft_print_bytes(dst_f, atoi(cmd->symbol), 15);
	fprintf(dst_f, "\n");
}

static void	ft_code_c(FILE* dst_f, t_cmd *cmd)
{
	int		cmp;
	int		dest;
	int		jmp;
	char	*isM;

	dest = 0;
	jmp = 0;
	isM = strstr(cmd->comp, "M");
	fprintf(dst_f, "111");
	if (isM)
	{
		fprintf(dst_f, "1");
		*isM = 'A';
	}
	else
		fprintf(dst_f, "0");
	if (!strcmp(cmd->comp, "0"))
		cmp = 42;
	else if (!strcmp(cmd->comp, "1"))
		cmp = 63;
	else if (!strcmp(cmd->comp, "-1"))
		cmp = 58;
	else if (!strcmp(cmd->comp, "D"))
		cmp = 12;
	else if (!strcmp(cmd->comp, "A"))
		cmp = 48;
	else if (!strcmp(cmd->comp, "!D"))
		cmp = 13;
	else if (!strcmp(cmd->comp, "!A"))
		cmp = 49;
	else if (!strcmp(cmd->comp, "-D"))
		cmp = 15;
	else if (!strcmp(cmd->comp, "-A"))
		cmp = 51;
	else if (!strcmp(cmd->comp, "D+1"))
		cmp = 31;
	else if (!strcmp(cmd->comp, "A+1"))
		cmp = 55;
	else if (!strcmp(cmd->comp, "D-1"))
		cmp = 14;
	else if (!strcmp(cmd->comp, "A-1"))
		cmp = 50;
	else if (!strcmp(cmd->comp, "D+A"))
		cmp = 2;
	else if (!strcmp(cmd->comp, "D-A"))
		cmp = 19;
	else if (!strcmp(cmd->comp, "A-D"))
		cmp = 7;
	else if (!strcmp(cmd->comp, "D&A"))
		cmp = 0;
	else // if (!strcmp(cmd->comp, "D|A"))
		cmp = 21;
	ft_print_bytes(dst_f, cmp, 6);

	if(cmd->dest)
	{
		if (strstr(cmd->dest, "M"))
			dest += 1;
		if (strstr(cmd->dest, "D"))
			dest += 2;
		if (strstr(cmd->dest, "A"))
			dest += 4;
	}
	ft_print_bytes(dst_f, dest, 3);

	if(cmd->jmp)
	{
		if(!strcmp(cmd->jmp, "JGT"))
			jmp = 1;
		else if(!strcmp(cmd->jmp, "JEQ"))
			jmp = 2;
		else if(!strcmp(cmd->jmp, "JGE"))
			jmp = 3;
		else if(!strcmp(cmd->jmp, "JLT"))
			jmp = 4;
		else if(!strcmp(cmd->jmp, "JNE"))
			jmp = 5;
		else if(!strcmp(cmd->jmp, "JLE"))
			jmp = 6;
		else if(!strcmp(cmd->jmp, "JMP"))
			jmp = 7;
	}
	ft_print_bytes(dst_f, jmp, 3);
	fprintf(dst_f, "\n");
}

void		code(FILE* dst_f, t_cmd **commands, t_sbl **symbols)
{
	t_cmd	*curr_cmd;
	curr_cmd = *commands;

	while(curr_cmd)
	{
		if (curr_cmd->type == 'A')
			ft_code_a(dst_f, curr_cmd, symbols);
		else if (curr_cmd->type == 'C')
			ft_code_c(dst_f, curr_cmd);
		curr_cmd = curr_cmd->next;
	}
}