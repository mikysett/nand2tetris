#include "jackCompiler.h"

static void	compile_from_sources(t_list *files_head);

int			main(int argc, char **argv)
{
	t_list		**src_files;

	if (argc != 2)
		return (print_error(ERR_NO_ARG));

	src_files = set_sources(argv[1], ".jack", ".vm");
	if (!src_files)
		return (0);
	compile_from_sources(*src_files);
	ft_free_sources(src_files);
	return (0);
}

static void	compile_from_sources(t_list *files_head)
{
	t_srcFile	*curr_file;
	FILE		*srcFile;
	FILE		*dstFile;
	t_list		**tokens;

	while (files_head)
	{
		curr_file = (t_srcFile *)files_head->content;
		DEBUG_CODE(printf("\nFile parsing: %s in %s\n",
			curr_file->src_name,
			curr_file->dst_name);)
		srcFile = fopen(curr_file->src_name, "r");
		dstFile = fopen(curr_file->dst_name, "w");
		if(!srcFile)
			print_error(ERR_OPEN_FAIL);
		if (!dstFile)
			print_error(ERR_DST_FILE);
		else
		{
			tokens = jackTokenizer(srcFile);
			compilationEngine(dstFile, *tokens);
			ft_free_tokens(tokens);
			fclose(srcFile);
			fclose(dstFile);
		}
		files_head = files_head->next;
	}
}