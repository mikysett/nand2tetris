/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str_arr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msessa <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 14:03:16 by msessa            #+#    #+#             */
/*   Updated: 2021/07/13 21:44:43 by msessa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_str_arr(char **str_arr)
{
	char	**o_str_arr;

	o_str_arr = str_arr;
	if (str_arr)
	{
		while (*str_arr)
		{
			free(*str_arr);
			str_arr++;
		}
		free(o_str_arr);
	}
}
