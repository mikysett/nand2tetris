#include "vmtranslator.h"

static int	ft_getlabel(int index)
{
	static int label = 0;

	if (!index)
	{
		label++;
		return (label - 1);
	}
	return (label);
}

static void	ft_push(FILE* dst_f, t_cmd *cmd, char *b_name)
{
	int address;

	if (!strcmp(cmd->arg1, "constant"))
	{
		fprintf(dst_f,
			"@%s\n"
			"D=A\n"
			"@SP\n"
			"M=M+1\n"
			"A=M-1\n"
			"M=D\n",
			cmd->arg2);
	}
	else if (!strcmp(cmd->arg1, "temp")
		|| !strcmp(cmd->arg1, "pointer"))
	{
		if (!strcmp(cmd->arg1, "temp"))
			address = 5;
		else // if (!strcmp(cmd->arg1, "pointer"))
			address = 3;
		address += atoi(cmd->arg2);
		fprintf(dst_f,
			"@%d\n"
			"D=M\n"
			"@SP\n"
			"M=M+1\n"
			"A=M-1\n"
			"M=D\n",
			address);
	}
	else if(!strcmp(cmd->arg1, "static"))
	{
		fprintf(dst_f,
			"@%s.%s\n"
			"D=M\n"
			"@SP\n"
			"M=M+1\n"
			"A=M-1\n"
			"M=D\n",
			b_name,
			cmd->arg2);
	}
	else
	{
		if (!strcmp(cmd->arg1, "local"))
			fprintf(dst_f, "@LCL\n");
		else if (!strcmp(cmd->arg1, "argument"))
			fprintf(dst_f, "@ARG\n");
		else if (!strcmp(cmd->arg1, "this"))
			fprintf(dst_f, "@THIS\n");
		else if (!strcmp(cmd->arg1, "that"))
			fprintf(dst_f, "@THAT\n");
		fprintf(dst_f,
			"D=M\n"
			"@%s\n"
			"A=D+A\n"
			"D=M\n"
			"@SP\n"
			"M=M+1\n"
			"A=M-1\n"
			"M=D\n",
			cmd->arg2);
	}
}

static void	ft_pop(FILE* dst_f, t_cmd *cmd, char *b_name)
{
	int address;

	if (!strcmp(cmd->arg1, "temp")
		|| !strcmp(cmd->arg1, "pointer"))
	{
		if (!strcmp(cmd->arg1, "temp"))
			address = 5;
		else // if (!strcmp(cmd->arg1, "pointer"))
			address = 3;
		address += atoi(cmd->arg2);
		fprintf(dst_f,
			"@SP\n"
			"AM=M-1\n"
			"D=M\n"
			"@%d\n"
			"M=D\n",
			address);
	}
	else if(!strcmp(cmd->arg1, "static"))
	{
		fprintf(dst_f,
			"@SP\n"
			"AM=M-1\n"
			"D=M\n"
			"@%s.%s\n"
			"M=D\n",
			b_name,
			cmd->arg2);
	}
	else
	{
		fprintf(dst_f,
			"@SP\n"
			"M=M-1\n"
			"@%s\n"
			"D=A\n",
			cmd->arg2);
		if (!strcmp(cmd->arg1, "local"))
			fprintf(dst_f, "@LCL\n");
		else if (!strcmp(cmd->arg1, "argument"))
			fprintf(dst_f, "@ARG\n");
		else if (!strcmp(cmd->arg1, "this"))
			fprintf(dst_f, "@THIS\n");
		else if (!strcmp(cmd->arg1, "that"))
			fprintf(dst_f, "@THAT\n");
		fprintf(dst_f,
			"D=D+M\n"		// The address of XXX[Y]
			"@SP\n"
			"A=M+1\n"
			"M=D\n"			// We save that address in SP[..+1]
			"@SP\n"
			"A=M\n"
			"D=M\n"			// We take the value to copy
			"A=A+1\n"
			"A=M\n"
			"M=D\n");		// We copy that value
	}
}

static void	ft_add(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"@SP\n"
		"A=M-1\n"
		"M=M+D\n");
}

static void	ft_sub(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"@SP\n"
		"A=M-1\n"
		"M=M-D\n");
}

static void	ft_neg(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"A=M-1\n"
		"M=-M\n");
}

static void	ft_eq(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"A=A-1\n"
		"D=D-M\n"
		"M=-1\n"
		"@LABEL%d\n"
		"D;JEQ\n"
		"@SP\n"
		"A=M-1\n"
		"M=0\n"
		"(LABEL%d)\n", // Second jump destination
		ft_getlabel(0),
		ft_getlabel(-1));
}

static void	ft_gt(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"A=A-1\n"
		"D=M-D\n"
		"M=-1\n"
		"@LABEL%d\n"
		"D;JGT\n"
		"@SP\n"
		"A=M-1\n"
		"M=0\n"
		"(LABEL%d)\n",
		ft_getlabel(0),
		ft_getlabel(-1));
}

static void	ft_lt(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"A=A-1\n"
		"D=M-D\n"
		"M=-1\n"
		"@LABEL%d\n"
		"D;JLT\n"
		"@SP\n"
		"A=M-1\n"
		"M=0\n"
		"(LABEL%d)\n",
		ft_getlabel(0),
		ft_getlabel(-1));
}

static void	ft_and(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"@SP\n"
		"A=M-1\n"
		"M=D&M\n");
}

static void	ft_or(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"@SP\n"
		"A=M-1\n"
		"M=D|M\n");
}

static void	ft_not(FILE* dst_f)
{
	fprintf(dst_f,
		"@SP\n"
		"A=M-1\n"
		"M=!M\n");
}

void		code(FILE* dst_f, t_cmd **commands, char *b_name)
{
	t_cmd	*cmd;

	cmd = *commands;
	while(cmd)
	{
		if (cmd->type == add)
			ft_add(dst_f);
		else if (cmd->type == sub)
			ft_sub(dst_f);
		else if (cmd->type == neg)
			ft_neg(dst_f);
		else if (cmd->type == eq)
			ft_eq(dst_f);
		else if (cmd->type == gt)
			ft_gt(dst_f);
		else if (cmd->type == lt)
			ft_lt(dst_f);
		else if (cmd->type == and)
			ft_and(dst_f);
		else if (cmd->type == or)
			ft_or(dst_f);
		else if (cmd->type == not)
			ft_not(dst_f);
		else if (cmd->type == push)
			ft_push(dst_f, cmd, b_name);
		else if (cmd->type == pop)
			ft_pop(dst_f, cmd, b_name);
		cmd = cmd->next;
	}
}