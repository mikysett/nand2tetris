// Big disclaimer:
// Focus was not on correctly freeing the memory
// This VMTranslator doesn't have a proper memory management
// Also error handling input is maintained at the bare minimum

// This version of the VM is meant to work on ex08
// To run the first 3 exercises of this chapter comment
// the ft_init_code(dst_f) in code.c

// To compile you can use make instruction
// To execute the program use ./vmtranslator <filename or dirname>
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
		printf("%10d: %2d | ", nb_l, curr->type);
		if (curr->arg1)
			printf("arg1: %20s | ", curr->arg1);
		if (curr->arg2)
			printf("arg2: %4s | ", curr->arg2);
		if (curr->file_name)
			printf("File: %s", curr->file_name);
		printf("\n");
		curr = curr->next;
		nb_l++;
	}
})

static int	ft_isextension(char *f, char *ext)
{
	int	f_len;
	int	ext_len;

	f_len = strlen(f);
	ext_len = strlen(ext);

	while (--f_len > 0 && --ext_len > 0)
		if (f[f_len] != ext[ext_len])
			return (0);
	return (1);
}

static char	*ft_set_dst_name(char *path, char *file)
{
	char	*dst_name;
	char	*ext_pos;
	int		name_end;

	if (ft_isextension(file, ".vm"))
	{
		dst_name = malloc(sizeof(char) * (strlen(path) + strlen(file) + 2));
		strcpy(dst_name, path);
		strcat(dst_name, file);
		ext_pos = strstr(dst_name, ".vm");
		ext_pos[1] = 'a';
		ext_pos[2] = 's';
		ext_pos[3] = 'm';
		ext_pos[4] = '\0';
	}
	else
	{
		dst_name = malloc(sizeof(char) * (strlen(path) + strlen(file) + 5));
		strcpy(dst_name, path);
		strcat(dst_name, file);
		name_end = strlen(dst_name);
		dst_name[name_end] = '.';
		dst_name[name_end + 1] = 'a';
		dst_name[name_end + 2] = 's';
		dst_name[name_end + 3] = 'm';
		dst_name[name_end + 4] = '\0';
	}
	return (dst_name);
}

static char	**ft_set_src_files(char *path, char *file)
{
	DIR				*src_dir;
	struct dirent	*file_name;
	char			src_files[2048];


	src_files[0] = '\0';
	if (ft_isextension(file, ".vm"))
		strcat(src_files, file);
	else
	{
		src_dir = opendir(path);
		if (src_dir)
		{
			while ((file_name = readdir(src_dir)))
			{
				if (!strcmp (file_name->d_name, ".")
					|| !strcmp (file_name->d_name, "..")
					|| !ft_isextension(file_name->d_name, ".vm"))
					continue;
				strcat(src_files, file_name->d_name);
				strcat(src_files, "#");
			}
		}
		closedir(src_dir);
	}
	return (ft_split(src_files, '#'));
}

static void	ft_free_cmd(t_cmd **cmd)
{
	t_cmd *curr;
	t_cmd *next;

	curr = *cmd;
	while (curr)
	{
		next = curr->next;
		if (curr->file_name)
			free(curr->file_name);
		if (curr->arg1)
			free(curr->arg1);
		if (curr->arg2)
			free(curr->arg2);
		free(curr);
		curr = next;
	}
	if (cmd)
		free(cmd);
}

int			main(int argc, char **argv)
{
	FILE	*srcFile;
	FILE	*dstFile;

	char	*path;
	char	*file_buff;
	char	*file;

	char	**src_files;
	char	*dst_name;
	char	file_sel[2048];
	int		i;

	t_cmd	**commands;
	int		parse_result;

	if (argc != 2)
		return (errorExit(ERR_NO_ARG));

	path = strdup(argv[1]);
	file_buff = strdup(argv[1]);
	file = basename(file_buff);
	if (ft_isextension(file, ".vm"))
		*(strstr(path, file)) = '\0';
	if (!(commands = malloc(sizeof(t_cmd *))))
			return (errorExit(-1));
	*commands = 0;
	src_files = ft_set_src_files(path, file);
	i = 0;
	while (src_files[i])
	{
		DEBUG_CODE(printf("\nFile parsing: %s%s\n", path, src_files[i]);)
		strcpy(file_sel, path);
		strcat(file_sel, src_files[i]);
		srcFile = fopen(file_sel, "r");
		if(!srcFile)
			return (errorExit(ERR_OPEN_FAIL));
		parse_result = parser(srcFile, commands, src_files[i]);
		fclose(srcFile);
		if (parse_result)
			return (errorExit(parse_result));
		i++;
	}

	DEBUG_CODE(ft_print_cmd(commands);)
 	dst_name = ft_set_dst_name(path, file);
	if (!dst_name)
		return (errorExit(ERR_WRONG_EXT));	
	dstFile = fopen(dst_name, "w");
	if (!dstFile)
		return (errorExit(ERR_DST_FILE));
	code(dstFile, commands);
	fclose(dstFile);

	ft_free_split(src_files);
	free(path);
	free(file_buff);
	free(dst_name);
	ft_free_cmd(commands);
	return (0);
}