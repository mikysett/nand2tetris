#include "vmtranslator.h"

// Read the next line and ignore empty lines and comments
// Do not cancel white spaces
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
		if (isspace(buf) && buf != ' ')
			buf = ' ';
		if ((buf == ' ' && code_line) || (buf != ' ' && !comment_line))
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

static void		ft_free_split(char **args)
{
	int i;

	if (args)
	{
		i = 0;
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
	}
}

static int		ft_count_args(char **args)
{
	int i;

	i = 0;
	if (args)
		while (args[i])
			i++;
	return (i);
}

static t_type	ft_set_type(char *type_str)
{
	if (!strcmp(type_str, "add"))
		return (add);
	else if (!strcmp(type_str, "sub"))
		return (sub);
	else if (!strcmp(type_str, "neg"))
		return (neg);
	else if (!strcmp(type_str, "eq"))
		return (eq);
	else if (!strcmp(type_str, "gt"))
		return (gt);
	else if (!strcmp(type_str, "lt"))
		return (lt);
	else if (!strcmp(type_str, "and"))
		return (and);
	else if (!strcmp(type_str, "or"))
		return (or);
	else if (!strcmp(type_str, "not"))
		return (not);

	else if (!strcmp(type_str, "push"))
		return (push);
	else // if (!strcmp(type_str, "pop"))
		return (pop);
}

static t_cmd	*add_cmd(char **args)
{
	int		nb_args;
	t_cmd	*cmd;

	nb_args = ft_count_args(args);
	if (!nb_args)
	{
		errorExit(-1);
		return (0);
	}
	cmd = malloc(sizeof(t_cmd));
	cmd->type = ft_set_type(args[0]);
	cmd->arg1 = 0;
	cmd->arg2 = 0;

	if (cmd->type == push || cmd->type == pop
		|| cmd->type == c_function || cmd->type == c_call)
	{
		cmd->arg1 = strdup(args[1]);
		cmd->arg2 = strdup(args[2]);
	}
	ft_free_split(args);
	return (cmd);
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
		new_cmd = add_cmd(ft_split(line, ' '));

		// To see trimmed lines
		DEBUG_CODE(printf("line %3d: %s\n", nb_l, line);)
		ft_lstadd_back(commands, new_cmd);
		nb_l++;
	}
	if (read_result == ERR_MAX_L_SIZE)
		return (ERR_MAX_L_SIZE);
	return (0);
}