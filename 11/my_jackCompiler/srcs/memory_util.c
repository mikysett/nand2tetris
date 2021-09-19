#include "jackCompiler.h"

static void	del_source(void *source_void);
static void	del_token(void *token_void);

void			ft_free_sources(t_list **src_files)
{
	if (src_files)
	{
		ft_lstclear(src_files, del_source);
		free(src_files);
	}
}

static void		del_source(void *source_void)
{
	t_srcFile	*source = (t_srcFile *)source_void;
	if (source)
	{
		if (source->src_name)
			free(source->src_name);
		if (source->dst_name)
			free(source->dst_name);
		free(source);
	}
}

void			ft_free_tokens(t_list **tokens)
{
	if (tokens)
	{
		ft_lstclear(tokens, del_token);
		free(tokens);
	}
}

static void		del_token(void *token_void)
{
	t_token	*token = (t_token *)token_void;

	if (token)
	{
		if (token->identifier)
			free(token->identifier);
		if (token->stringval)
			free(token->stringval);
		free(token);
	}
}