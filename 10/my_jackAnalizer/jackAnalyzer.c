#include "jackAnalyzer.h"

int					errorExit(t_err err)
{
	switch (err)
	{
		case ERR_NO_ARG:
			printf("Error: Please provide a .jack file or directory\n");
			return (0);
		case ERR_OPEN_FAIL:
			perror("error");
			// printf("Error: Can't open the file\n");
			return (0);
		case ERR_OPEN_DIR_FAIL:
			printf("Error: Can't open the directory\n");
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

static void			ft_free_sources(t_srcFile **src_files)
{
	t_srcFile *curr;
	t_srcFile *next;

	if (src_files)
	{
		curr = *src_files;
		while (curr)
		{
			next = curr->next;
			if (curr->src_name)
				free(curr->src_name);
			if (curr->dst_name)
				free(curr->dst_name);
			free(curr);
			curr = next;
		}
		free(src_files);
	}
}

static void			ft_free_tokens(t_token_lst **tokens)
{
	t_token_lst *curr;
	t_token_lst *next;

	if (tokens)
	{
		curr = *tokens;
		while (curr)
		{
			next = curr->next;
			if (curr->identifier)
				free(curr->identifier);
			if (curr->stringval)
				free(curr->stringval);
			free(curr);
			curr = next;
		}
		free(tokens);
	}
}

static int			ft_isextension(char *f, char *ext)
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

static char			*set_ext_name(char *src, char *ext_in, char *ext_out)
{
	char	*dst_name;
	char	*ext_pos;
	size_t	ext_in_len;
	size_t	ext_out_len;
	size_t	ext_max_len;

	ext_in_len = strlen(ext_in);
	ext_out_len = strlen(ext_out);
	ext_max_len = ext_in_len < ext_out_len ? ext_out_len : ext_in_len;
	dst_name = malloc(sizeof(char) * (strlen(src) + ext_max_len + 1));
	if (!dst_name)
		return (0);
	strcpy(dst_name, src);
	ext_pos = strstr(dst_name, ext_in);
	if (!ext_pos)
		return (0);
	strcpy(ext_pos, ext_out);
	return (dst_name);
}

static void			ft_lstadd_back(t_srcFile **lst, t_srcFile *new)
{
	t_srcFile *curr;

	curr = *lst;
	if (!curr)
	{
		*lst = new;
		return ;
	}
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

static t_srcFile	*add_file(char *path, char *file_name)
{
	t_srcFile *new_file;

	new_file = malloc(sizeof(t_srcFile));
	if (!new_file)
		return (0);
	new_file->src_name = malloc(sizeof(char) * (strlen(path) + strlen(file_name) + 1));
	if (!new_file->src_name)
		return (0);
	strcpy(new_file->src_name, path);
	strcat(new_file->src_name, file_name);
	new_file->dst_name = set_ext_name(new_file->src_name, ".jack", ".xml");
	new_file->next = 0;
	return (new_file);
}

static void			set_sources(t_srcFile **src_files, char *path, char *ext)
{
	DIR				*src_dir;
	struct dirent	*file_name;

	src_dir = opendir(path);
	if (!src_dir)
	{
		errorExit(ERR_OPEN_DIR_FAIL);
		return ;
	}
	while ((file_name = readdir(src_dir)))
	{
		if (!strcmp (file_name->d_name, ".") || !strcmp (file_name->d_name, "..")
			|| !ft_isextension(file_name->d_name, ext))
			continue;
		ft_lstadd_back(src_files, add_file(path, file_name->d_name));
	}
	closedir(src_dir);
}

int			main(int argc, char **argv)
{
	FILE		*srcFile;
	FILE		*dstFile;
	t_srcFile	**src_files;
	t_srcFile	*file_sel;
	t_token_lst **tokens;

	if (!(src_files = malloc(sizeof(t_srcFile *))))
		return (0);
	*src_files = 0;
	if (argc != 2)
		return (errorExit(ERR_NO_ARG));
	if (ft_isextension(argv[1], ".jack"))
		*src_files = add_file("", argv[1]);
	else
		set_sources(src_files, argv[1], ".jack");

	file_sel = *src_files;
	while (file_sel)
	{
		DEBUG_CODE(printf("\nFile parsing: %s in %s\n",
			file_sel->src_name,
			file_sel->dst_name);)
		srcFile = fopen(file_sel->src_name, "r");
		if(!srcFile)
			return (errorExit(ERR_OPEN_FAIL));
		dstFile = fopen(file_sel->dst_name, "w");
		if (!dstFile)
			return (errorExit(ERR_DST_FILE));
		tokens = jackTokenizer(srcFile);
		DEBUG_CODE(printf("<tokens>\n");)
		compilationEngine(dstFile, *tokens);
		DEBUG_CODE(printf("</tokens>\n");)
		ft_free_tokens(tokens);
		fclose(srcFile);
		fclose(dstFile);
		file_sel = file_sel->next;
	}
	ft_free_sources(src_files);
	return (0);
}