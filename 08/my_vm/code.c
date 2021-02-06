#include "vmtranslator.h"

static int	ft_get_label(int index)
{
	static int label = 0;

	if (!index)
	{
		label++;
		return (label - 1);
	}
	return (label);
}

static int	ft_get_ret_address(int index)
{
	static int ret_address = 0;

	if (!index)
	{
		ret_address++;
		return (ret_address - 1);
	}
	if (index == -2)
		return (ret_address - 1);
	return (ret_address);
}

static void	ft_print_segment(FILE *dst_f, char *seg)
{
	if (!strcmp(seg, "local"))
		fprintf(dst_f,
			"@LCL\n");
	else if (!strcmp(seg, "argument"))
		fprintf(dst_f,
			"@ARG\n");
	else if (!strcmp(seg, "this"))
		fprintf(dst_f,
			"@THIS\n");
	else // if (!strcmp(seg, "that"))
		fprintf(dst_f,
			"@THAT\n");
}

static void	ft_push(FILE* dst_f, t_cmd *cmd, t_cmd *next_cmd)
{
	int address;

	DEBUG_CODE(fprintf(dst_f, "//---> PUSH %s %s\n", cmd->arg1, cmd->arg2);)
	if (!strcmp(cmd->arg1, "constant"))
	{
		if (!strcmp(cmd->arg2, "0") || !strcmp(cmd->arg2, "1"))
		{
			fprintf(dst_f,
				"@SP\n");
			if (!next_cmd || next_cmd->type != pop)
				fprintf(dst_f,
					"M=M+1\n"
					"A=M-1\n");
			else
				fprintf(dst_f,
					"A=M\n");
			fprintf(dst_f,
				"M=%s\n",
				cmd->arg2);
		}
		else
		{
			fprintf(dst_f,
				"@%s\n"
				"D=A\n"
				"@SP\n",
				cmd->arg2);
			if (!next_cmd || next_cmd->type != pop)
				fprintf(dst_f,
					"M=M+1\n"
					"A=M-1\n");
			else
				fprintf(dst_f,
					"A=M\n");
			fprintf(dst_f,
				"M=D\n");
		}
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
			"@SP\n",
			address);
		if (!next_cmd || next_cmd->type != pop)
			fprintf(dst_f,
				"M=M+1\n"
				"A=M-1\n");
		else
			fprintf(dst_f,
				"A=M\n");
		fprintf(dst_f,
			"M=D\n");
	}
	else if(!strcmp(cmd->arg1, "static"))
	{
		fprintf(dst_f,
			"@%s.%s\n"
			"D=M\n"
			"@SP\n",
			cmd->file_name,
			cmd->arg2);
		if (!next_cmd || next_cmd->type != pop)
			fprintf(dst_f,
				"M=M+1\n"
				"A=M-1\n");
		else
			fprintf(dst_f,
				"A=M\n");
		fprintf(dst_f,
			"M=D\n");
	}
	else // if arg1 is ARG LCL THIS THAT AND SO ON
	{
		ft_print_segment(dst_f, cmd->arg1);
		if (!strcmp(cmd->arg2, "0"))
			fprintf(dst_f,
				"A=M\n");
		else if (!strcmp(cmd->arg2, "1"))
			fprintf(dst_f,
				"A=M+1\n");
		else
			fprintf(dst_f,
				"D=M\n"
				"@%s\n"
				"A=D+A\n",
				cmd->arg2);
		fprintf(dst_f,
			"D=M\n"
			"@SP\n");
		if (!next_cmd || next_cmd->type != pop)
			fprintf(dst_f,
				"M=M+1\n"
				"A=M-1\n");
		else
			fprintf(dst_f,
				"A=M\n");
		fprintf(dst_f,
			"M=D\n");
	}
}

static void	ft_pop(FILE* dst_f, t_cmd *cmd, t_cmd *prev_cmd)
{
	int address;

	DEBUG_CODE(fprintf(dst_f, "//---> POP %s %s\n", cmd->arg1, cmd->arg2);)
	if (!strcmp(cmd->arg1, "temp")
		|| !strcmp(cmd->arg1, "pointer"))
	{
		if (!strcmp(cmd->arg1, "temp"))
			address = 5;
		else // if (!strcmp(cmd->arg1, "pointer"))
			address = 3;
		address += atoi(cmd->arg2);
		if(!prev_cmd || prev_cmd->type != push)
			fprintf(dst_f,
				"@SP\n"
				"AM=M-1\n"
				"D=M\n");
		fprintf(dst_f,
			"@%d\n"
			"M=D\n",
			address);
	}
	else if(!strcmp(cmd->arg1, "static"))
	{
		if(!prev_cmd || prev_cmd->type != push)
			fprintf(dst_f,
				"@SP\n"
				"AM=M-1\n"
				"D=M\n");
		fprintf(dst_f,
			"@%s.%s\n"
			"M=D\n",
			cmd->file_name,
			cmd->arg2);
	}
	else
	{
		if (!strcmp(cmd->arg2, "0"))
		{
			if(!prev_cmd || prev_cmd->type != push)
				fprintf(dst_f,
					"@SP\n"
					"AM=M-1\n"
					"D=M\n");
			ft_print_segment(dst_f, cmd->arg1);
			fprintf(dst_f,
				"A=M\n"
				"M=D\n");
		}
		else if (!strcmp(cmd->arg2, "1"))
		{
			if(!prev_cmd || prev_cmd->type != push)
				fprintf(dst_f,
					"@SP\n"
					"AM=M-1\n"
					"D=M\n");
			ft_print_segment(dst_f, cmd->arg1);
			fprintf(dst_f,
				"A=M+1\n"
				"M=D\n");
		}
		else
		{
				ft_print_segment(dst_f, cmd->arg1);
				fprintf(dst_f,
					"D=M\n"
					"@%s\n"
					"D=D+A\n"

					"@R14\n"
					"M=D\n"
					
					"@SP\n",
					cmd->arg2);
				if(!prev_cmd || prev_cmd->type != push)
					fprintf(dst_f,
						"AM=M-1\n");
				else
					fprintf(dst_f,
						"A=M\n");
				fprintf(dst_f,
					"D=M\n"
					"@R14\n"
					"A=M\n"
					"M=D\n");
				ft_print_segment(dst_f, cmd->arg1);
		}
	}
}

static void	ft_add(FILE* dst_f)
{
	DEBUG_CODE(fprintf(dst_f, "//---> ADD\n");)
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
	DEBUG_CODE(fprintf(dst_f, "//---> SUBSTRACTION\n");)
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
	DEBUG_CODE(fprintf(dst_f, "//---> NEGATE\n");)
	fprintf(dst_f,
		"@SP\n"
		"A=M-1\n"
		"M=-M\n");
}

static void	ft_eq(FILE* dst_f)
{
	DEBUG_CODE(fprintf(dst_f, "//---> EQUAL\n");)
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
		"(LABEL%d)\n",
		ft_get_label(0),
		ft_get_label(-1));
}

static void	ft_gt(FILE* dst_f)
{
	DEBUG_CODE(fprintf(dst_f, "//---> GREATER THAN\n");)
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
		ft_get_label(0),
		ft_get_label(-1));
}

static void	ft_lt(FILE* dst_f)
{
	DEBUG_CODE(fprintf(dst_f, "//---> LESS THAN\n");)
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
		ft_get_label(0),
		ft_get_label(-1));
}

static void	ft_and(FILE* dst_f)
{
	DEBUG_CODE(fprintf(dst_f, "//---> AND\n");)
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
	DEBUG_CODE(fprintf(dst_f, "//---> OR\n");)
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
	DEBUG_CODE(fprintf(dst_f, "//---> NOT\n");)
	fprintf(dst_f,
		"@SP\n"
		"A=M-1\n"
		"M=!M\n");
}

static void	ft_c_label(FILE* dst_f, t_cmd *cmd, char *ft_name)
{
	DEBUG_CODE(fprintf(dst_f, "//---> LABEL\n");)
	fprintf(dst_f,
		"(%s$%s)\n",
		ft_name,
		cmd->arg1);
}

static void	ft_c_goto(FILE* dst_f, t_cmd *cmd, char *ft_name)
{
	DEBUG_CODE(fprintf(dst_f, "//---> GOTO\n");)
	fprintf(dst_f,
		"@%s$%s\n"
		"0;JMP\n",
		ft_name,
		cmd->arg1);
}

static void	ft_c_if_goto(FILE* dst_f, t_cmd *cmd, char *ft_name)
{
	DEBUG_CODE(fprintf(dst_f, "//---> IF GOTO\n");)
	fprintf(dst_f,
		"@SP\n"
		"AM=M-1\n"
		"D=M\n"
		"@%s$%s\n"
		"D;JNE\n",
		ft_name,
		cmd->arg1);
}

static char	*ft_c_function(FILE* dst_f, t_cmd *cmd, char *ft_name)
{
	int locals;

	DEBUG_CODE(fprintf(dst_f, "//---> FUNCTION\n");)
	free(ft_name);
	ft_name = strdup(cmd->arg1);
	locals = atoi(cmd->arg2);
	fprintf(dst_f,
		"(%s)\n",
		ft_name);
	if (locals)
	{
		if (locals <= 2)
		{
			fprintf(dst_f,
				"@SP\n"
				"M=M+1\n"
				"A=M-1\n"
				"M=0\n");
			if (locals == 2)
				fprintf(dst_f,
					"@SP\n"
					"M=M+1\n"
					"A=M-1\n"
					"M=0\n");
		}
		else
		{
			fprintf(dst_f,
				"@SP\n"
				"A=M\n");
			while (locals)
			{
				fprintf(dst_f,
					"M=0\n"
					"A=A+1\n");
				locals--;
			}
			fprintf(dst_f,
				"D=A\n"
				"@SP\n"
				"M=D\n");
		}
	}
	return (ft_name);
}

static void ft_c_return(FILE* dst_f)
{
	DEBUG_CODE(fprintf(dst_f, "//---> RETURN\n");)
	fprintf(dst_f,
		"@$RETURN$REF\n"
		"0;JMP\n");
}

static void ft_c_call(FILE* dst_f, t_cmd *cmd)
{
	int		arg_position;

	// OLD IMPLEMENTATION
	// fprintf(dst_f,
	// 	"@RET_ADDRESS_%d\n"
	// 	"D=A\n"
	// 	"@SP\n"
	// 	"M=M+1\n"
	// 	"A=M-1\n"
	// 	"M=D\n"

	// 	"@LCL\n"
	// 	"D=M\n"
	// 	"@SP\n"
	// 	"M=M+1\n"
	// 	"A=M-1\n"
	// 	"M=D\n"

	// 	"@ARG\n"
	// 	"D=M\n"
	// 	"@SP\n"
	// 	"M=M+1\n"
	// 	"A=M-1\n"
	// 	"M=D\n"

	// 	"@THIS\n"
	// 	"D=M\n"
	// 	"@SP\n"
	// 	"M=M+1\n"
	// 	"A=M-1\n"
	// 	"M=D\n"

	// 	"@THAT\n"
	// 	"D=M\n"
	// 	"@SP\n"
	// 	"M=M+1\n"
	// 	"A=M-1\n"
	// 	"M=D\n"

	// 	"@SP\n"
	// 	"D=M\n"
	// 	"@LCL\n"
	// 	"M=D\n"
	// 	"@%d\n"
	// 	"D=D-A\n"
	// 	"@ARG\n"
	// 	"M=D\n"

	// 	"@%s\n"
	// 	"0;JMP\n"
	// 	"(RET_ADDRESS_%d)\n",
	// 	ft_get_ret_address(0),
	// 	arg_position,
	// 	cmd->arg1,
	// 	ft_get_ret_address(-1));

	DEBUG_CODE(fprintf(dst_f, "//---> CALL %s %s\n", cmd->arg1, cmd->arg2);)
	arg_position = atoi(cmd->arg2) + 5;
	fprintf(dst_f,
		"@RET_ADDRESS_%d\n"
		"D=A\n"
		"@SP\n"
		"M=M+1\n"
		"A=M-1\n"
		"M=D\n"

		"@$CALL_REF$BACK_%d\n"
		"D=A\n"
		"@R14\n"
		"M=D\n"

		"@$CALL$REF\n"
		"0;JMP\n"

		"($CALL_REF$BACK_%d)\n",
		ft_get_ret_address(0),
		ft_get_ret_address(-1),
		ft_get_ret_address(-1));


	fprintf(dst_f,
		"@%d\n"
		"D=D-A\n"
		"@ARG\n"
		"M=D\n"

		"@%s\n"
		"0;JMP\n"
		"(RET_ADDRESS_%d)\n",
		arg_position,
		cmd->arg1,
		ft_get_ret_address(-2));
}

static void	ft_init_code(FILE* dst_f)
{
	DEBUG_CODE(fprintf(dst_f, "//---> init code\n");)
	fprintf(dst_f,
		"@256\n"
		"D=A\n"
		"@SP\n"
		"M=D\n"

		"@RET_ADDRESS_SYS_INIT\n"
		"D=A\n"
		"@SP\n"
		"M=M+1\n"
		"A=M-1\n"
		"M=D\n"

		"@$CALL_REF$BACK_SYS_INIT\n"
		"D=A\n"
		"@R14\n"
		"M=D\n"

		"@$CALL$REF\n"
		"0;JMP\n"

		"($CALL_REF$BACK_SYS_INIT)\n"

		"@LCL\n"
		"M=D\n"
		"@5\n"
		"D=D-A\n"
		"@ARG\n"
		"M=D\n"

		"@Sys.init\n"
		"0;JMP\n"
		"(RET_ADDRESS_SYS_INIT)\n");

	DEBUG_CODE(fprintf(dst_f, "//---> CALL REFERENCE\n");)
	fprintf(dst_f,
		"($CALL$REF)\n"

		"@LCL\n"
		"D=M\n"
		"@SP\n"
		"M=M+1\n"
		"A=M-1\n"
		"M=D\n"

		"@ARG\n"
		"D=M\n"
		"@SP\n"
		"M=M+1\n"
		"A=M-1\n"
		"M=D\n"

		"@THIS\n"
		"D=M\n"
		"@SP\n"
		"M=M+1\n"
		"A=M-1\n"
		"M=D\n"

		"@THAT\n"
		"D=M\n"
		"@SP\n"
		"M=M+1\n"
		"A=M-1\n"
		"M=D\n"

		"@SP\n"
		"D=M\n"
		"@LCL\n"
		"M=D\n"

		"@R14\n"
		"A=M\n"
		"0;JMP\n");
	
	DEBUG_CODE(fprintf(dst_f, "//---> RETURN REFERENCE\n");)
	fprintf(dst_f,
		"($RETURN$REF)\n"

		"@LCL\n"
		"D=M\n"
		"@5\n"
		"A=D-A\n"
		"D=M\n"
		"@R13\n"
		"M=D\n"

		"@SP\n"
		"A=M-1\n"
		"D=M\n"
		"@ARG\n"
		"A=M\n"
		"M=D\n"
		"@ARG\n"
		"D=M+1\n"
		"@SP\n"
		"M=D\n"
		
		"@LCL\n"
		"AM=M-1\n"
		"D=M\n"
		"@THAT\n"
		"M=D\n"

		"@LCL\n"
		"AM=M-1\n"
		"D=M\n"
		"@THIS\n"
		"M=D\n"
		
		"@LCL\n"
		"AM=M-1\n"
		"D=M\n"
		"@ARG\n"
		"M=D\n"

		"@LCL\n"
		"AM=M-1\n"
		"D=M\n"
		"@LCL\n"
		"M=D\n"

		"@R13\n"
		"A=M\n"
		"0;JMP\n");
}

void		code(FILE* dst_f, t_cmd **commands)
{
	char	*function_name;
	t_cmd	*cmd;
	t_cmd	*next_cmd;
	t_cmd	*prev_cmd;

	function_name = strdup("Sys.init");
	ft_init_code(dst_f);
	cmd = *commands;
	prev_cmd = 0;
	while(cmd)
	{
		next_cmd = cmd->next;
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
			ft_push(dst_f, cmd, next_cmd);
		else if (cmd->type == pop)
			ft_pop(dst_f, cmd, prev_cmd);
		else if (cmd->type == c_label)
			ft_c_label(dst_f, cmd, function_name);
		else if (cmd->type == c_goto)
			ft_c_goto(dst_f, cmd, function_name);
		else if (cmd->type == c_if_goto)
			ft_c_if_goto(dst_f, cmd, function_name);
		else if (cmd->type == c_function)
			function_name = ft_c_function(dst_f, cmd, function_name);
		else if (cmd->type == c_call)
			ft_c_call(dst_f, cmd);
		else if (cmd->type == c_return)
			ft_c_return(dst_f);
		prev_cmd = cmd;
		cmd = next_cmd;
	}
	free(function_name);
}