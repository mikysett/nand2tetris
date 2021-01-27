#include "assembler.h"

// Read the next line and ignore empty lines, comments and spaces
static int	ft_readline(FILE *srcFile, char *line)
{
	char	buf;
	int		code_line;
	int		comment_line;
	int		i;

	i = 0;
	code_line = 0;
	comment_line = 0;
	while (fread(&buf, 1, 1, srcFile))
	{
		if (buf == '\n')
		{
			if (!code_line)
			{
				comment_line = 0;
				continue ;
			}
			break ;
		}
		if (!comment_line && i >= 1
			&& buf == '/' && line[i - 1] == '/')
		{
			line[i - 1] = '\0';
			i--;
			comment_line = 1;
			if (!i)
				code_line = 0;
		}
		if (!isspace(buf) && !comment_line)
		{
			if (!code_line)
				code_line = 1;
			line[i] = buf;
			i++;
			if (i == MAX_LINE_SIZE)
				return (ERR_MAX_L_SIZE);
		}
	}
	line[i] = '\0';
	return (0);
}

static void		ft_lstadd_back(t_cmd **lst, t_cmd *new)
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

static t_cmd	*add_command_a(char *line)
{
	t_cmd *new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	new_cmd->type = 'A';
	line++;
	new_cmd->symbol = strdup(line);
	new_cmd->dest = 0;
	new_cmd->comp = 0;
	new_cmd->jmp = 0;
	return (new_cmd);
}

static t_cmd	*add_command_l(char *line)
{
	t_cmd *new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	new_cmd->type = 'L';
	line++;
	line[strlen(line) - 1] = '\0';
	new_cmd->symbol = strdup(line);
	new_cmd->dest = 0;
	new_cmd->comp = 0;
	new_cmd->jmp = 0;
	return (new_cmd);
}

static t_cmd	*add_command_c(char *line)
{
	t_cmd	*new_cmd;
	char	*isComp;
	char	*isJump;

	new_cmd = malloc(sizeof(t_cmd));
	isComp = strstr(line, "=");
	isJump = strstr(line, ";");
	new_cmd->type = 'C';
	new_cmd->symbol = 0;

	if (isComp)
	{
		new_cmd->dest = strndup(line, isComp - line);
		line = isComp + 1;
	}
	else
		new_cmd->dest = 0;
	if (isJump)
	{
		new_cmd->comp = strndup(line, isJump - line);
		line = isJump + 1;
		new_cmd->jmp = strdup(line);
	}
	else
	{
		new_cmd->comp = strdup(line);
		new_cmd->jmp = 0;
	}
	return (new_cmd);
}

int				parser(FILE *srcFile, t_cmd **commands)
{
	char	line[MAX_LINE_SIZE];
	int		read_result;
	t_cmd	*new_cmd;
	int		nb_l;

	nb_l = 0;
	while (!(read_result = ft_readline(srcFile, line)))
	{
		if (line[0] == '\0')
			break ;
		// To see trimmed lines
		DEBUG_CODE(printf("line %3d: %s\n", nb_l, line);)
		if (line[0] == '@')
			new_cmd = add_command_a(line);
		else if (line[0] == '(')
			new_cmd = add_command_l(line);
		else
			new_cmd = add_command_c(line);
		ft_lstadd_back(commands, new_cmd);
		nb_l++;
	}
	if (read_result == ERR_MAX_L_SIZE)
		return (ERR_MAX_L_SIZE);
	return (0);
}