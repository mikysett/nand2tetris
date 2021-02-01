// Big disclaimer:
// Focus was not on correctly freeing the memory
// This VMTranslator doesn't have a proper memory management
// Also error handling input is maintained at the bare minimum

// This version of the VM is meant to work on ex07
// The one in next chapter will be the full VM

// Namely this VM doesn't handle multiple files sources

// To compile you can use make instruction
// To execute the program use ./vmtranslator <filename>
// To see verbose output switch DEBUG_MODE to 1 in vmtranslator.h

#include "vmtranslator.h"

int			errorExit(int err)
{
	switch (err)
	{
		case ERR_NO_ARG:
			printf("Error: Please provide one .vm file as argument\n");
			return (0);
		case ERR_OPEN_FAIL:
			printf("Error: Can't open the file\n");
			return (0);
		case ERR_WRONG_EXT:
			printf("Error: Input file must have .vm extension\n");
			return (0);
		case ERR_DST_FILE:
			printf("Error: Can't create output file .hack\n");
			return (0);
		default:
			printf("Error: Unknown error occurred\n");
			return (0);
	}
}

static void	ft_print_cmd(t_cmd **commands)
{
	t_cmd *curr;
	int nb_l;

	printf("\n---- COMMANDS IN MEMORY\n");
	curr = *commands;
	nb_l = 0;
	while (curr)
	{
		printf("%10d: %2d | ", nb_l, curr->type);
		if (curr->arg1)
			printf("arg1: %10s | ", curr->arg1);
		if (curr->arg2)
			printf("arg2: %4s", curr->arg2);
		printf("\n");
		curr = curr->next;
		nb_l++;
	}
}

static char	*set_dst_name(char *src)
{
	char	*dst_name;
	char	*ext_pos;

	dst_name = malloc(sizeof(char) * (strlen(src) + 2));
	strcpy(dst_name, src);
	ext_pos = strstr(dst_name, ".vm");
	if (!ext_pos)
		return (0);
	ext_pos[1] = 'a';
	ext_pos[2] = 's';
	ext_pos[3] = 'm';
	ext_pos[4] = '\0';
	return (dst_name);
}

static char	*ft_remove_ext(char *str)
{
	int	str_len;

	str_len = strlen(str) - 1;
	while (str_len > 0)
	{
		if (str[str_len] == '.')
		{
			str[str_len] = '\0';
			return (str);
		}
		str_len--;
	}
	return (str);
}

int			main(int argc, char **argv)
{
	FILE	*srcFile;
	FILE	*dstFile;
	char	*dst_name;
	char	*b_name;
	t_cmd	**commands;
	int		parse_result;

	if (argc != 2)
		return (errorExit(ERR_NO_ARG));
 	dst_name = set_dst_name(argv[1]);
	if (!dst_name)
		return (errorExit(ERR_WRONG_EXT));
	dstFile = fopen(dst_name, "w");
	if (!dstFile)
		return (errorExit(ERR_DST_FILE));

	commands = malloc(sizeof(t_cmd *));

	srcFile = fopen(argv[1], "r");
	if(!srcFile)
		return (errorExit(ERR_OPEN_FAIL));

	parse_result = parser(srcFile, commands);
	fclose(srcFile);
	if (parse_result)
		return (errorExit(parse_result));

	DEBUG_CODE(ft_print_cmd(commands);)
	b_name = ft_remove_ext(basename(argv[1]));
	code(dstFile, commands, b_name);
	free(dst_name);
	fclose(dstFile);
	return (0);
}