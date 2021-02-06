// Big disclaimer:
// Focus was not on correctly freeing the memory
// This VMTranslator doesn't have a proper memory management
// Also error handling input is maintained at the bare minimum

// To compile you can use make instruction
// To execute the program use ./assembler <filename>
// To see verbose output switch DEBUG_MODE to 1 in assembler.h

#include "assembler.h"

static int	errorExit(int err)
{
	switch (err)
	{
		case ERR_NO_ARG:
			printf("Error: Please provide one .asm file as argument\n");
			return (0);
		case ERR_OPEN_FAIL:
			printf("Error: Can't open the file\n");
			return (0);
		case ERR_WRONG_EXT:
			printf("Error: Input file must have .asm extension\n");
			return (0);
		case ERR_DST_FILE:
			printf("Error: Can't create output file .hack\n");
			return (0);
		default:
			printf("Error: Unknown error occurred\n");
			return (0);
	}
}

DEBUG_CODE(
static void	ft_print_cmd(t_cmd **commands)
{
	t_cmd *curr;
	int nb_l;

	printf("\n---- COMMANDS IN MEMORY\n");
	curr = *commands;
	nb_l = 0;
	while (curr)
	{
		printf("%10d: %c\n", nb_l, curr->type);
		if (curr->symbol)
			printf("symbol    : %s\n", curr->symbol);
		else
		{
			if (curr->dest)
				printf("dest      : %s\n", curr->dest);
			if (curr->comp)
				printf("comp      : %s\n", curr->comp);
			if (curr->jmp)
				printf("jmp       : %s\n", curr->jmp);
		}
		printf("\n");
		curr = curr->next;
		nb_l++;
	}
})

static char	*set_dst_name(char *src)
{
	char	*dst_name;
	char	*ext_pos;

	dst_name = malloc(sizeof(char) * (strlen(src) + 2));
	strcpy(dst_name, src);
	ext_pos = strstr(dst_name, ".asm");
	if (!ext_pos)
		return (0);
	ext_pos[1] = 'h';
	ext_pos[2] = 'a';
	ext_pos[3] = 'c';
	ext_pos[4] = 'k';
	ext_pos[5] = '\0';
	return (dst_name);
}

static void	ft_free_cmd(t_cmd **cmd)
{
	t_cmd *curr;
	t_cmd *next;

	curr = *cmd;
	while (curr)
	{
		next = curr->next;
		if (curr->symbol)
			free(curr->symbol);
		if (curr->dest)
			free(curr->dest);
		if (curr->comp)
			free(curr->comp);
		if (curr->jmp)
			free(curr->jmp);
		free(curr);
		curr = next;
	}
	if (cmd)
		free(cmd);
}

static void	ft_free_sbl(t_sbl **sbl)
{
	t_sbl *curr;
	t_sbl *next;

	curr = *sbl;
	while (curr)
	{
		next = curr->next;
		if (curr->symbol)
			free(curr->symbol);
		free(curr);
		curr = next;
	}
	if (sbl)
		free(sbl);
}

int			main(int argc, char **argv)
{
	FILE	*srcFile;
	FILE	*dstFile;
	char	*dst_name;
	t_cmd	**commands;
	t_sbl	**symbols;
	int		parse_result;

	if (argc != 2)
		return (errorExit(ERR_NO_ARG));
	dst_name = set_dst_name(argv[1]);
	if (!dst_name)
		return (errorExit(ERR_WRONG_EXT));

	commands = malloc(sizeof(t_cmd *));
	symbols = malloc(sizeof(t_sbl *));

	srcFile = fopen(argv[1], "r");
	if(!srcFile)
		return (errorExit(ERR_OPEN_FAIL));

	parse_result = parser(srcFile, commands);
	fclose(srcFile);
	if (parse_result)
		return (errorExit(parse_result));

	DEBUG_CODE(ft_print_cmd(commands);)
	symbol_table(commands, symbols);


	dstFile = fopen(dst_name, "w");
	free(dst_name);
	if (!dstFile)
		return (errorExit(ERR_DST_FILE));
	code(dstFile, commands, symbols);
	fclose(dstFile);
	
	ft_free_cmd(commands);
	ft_free_sbl(symbols);
	return (0);
}