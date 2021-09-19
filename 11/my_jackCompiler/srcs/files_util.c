#include "jackCompiler.h"

static int			ft_isextension(char *f, char *ext);
static t_srcFile	*add_file(char *path, char *file_name,
						char *ext_in, char *ext_out);
static char			*set_ext_name(char *src, char *ext_in, char *ext_out);

t_list			**set_sources(char *path, char *ext_in, char *ext_out)
{
	DIR				*src_dir;
	struct dirent	*file_name;
	t_list			**src_files;

	if (!(src_files = calloc(1, sizeof(t_srcFile *))))
		return (0);
	if (ft_isextension(path, ext_in))
		ft_lstadd_back(src_files, ft_lstnew(add_file("", path, ext_in, ext_out)));
	else
	{
		src_dir = opendir(path);
		if (!src_dir)
		{
			print_error(ERR_OPEN_DIR_FAIL);
			return (src_files);
		}
		while ((file_name = readdir(src_dir)))
		{
			if (ft_isextension(file_name->d_name, ext_in))
				ft_lstadd_back(src_files, ft_lstnew(add_file(path,
				file_name->d_name, ext_in, ext_out)));
		}
		closedir(src_dir);
	}
	return (src_files);
}

static int			ft_isextension(char *f, char *ext)
{
	if (strlen(f) <= strlen(ext))
		return (0);
	while (*f && *f != '.')	
		f++;
	if (!strcmp(f, ext))
		return (1);
	return (0);
}

static t_srcFile	*add_file(char *path, char *file_name,
						char *ext_in, char *ext_out)
{
	t_srcFile *new_file;

	new_file = malloc(sizeof(t_srcFile));
	if (!new_file)
		return (0);
	new_file->src_name = malloc((strlen(path) + strlen(file_name) + 1));
	if (!new_file->src_name)
		return (0);
	strcpy(new_file->src_name, path);
	strcat(new_file->src_name, file_name);
	new_file->dst_name = set_ext_name(new_file->src_name, ext_in, ext_out);
	return (new_file);
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

