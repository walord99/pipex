/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 02:15:04 by bplante           #+#    #+#             */
/*   Updated: 2023/11/20 03:30:17 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// once at none space char
// add 1 to len until none escaped space and depth == 0 space

#include "pipex.h"


//TODO, GO END WORD, BY SPLITTING THIS INTO USEFULL FUNCTION LIKE, BREAKOUT OF LITTERAL OR SMTH
int	get_arg_len(char *str)
{
	int		i;
	bool	is_escaped;
	int		litteral_depth;
	char	current_lit;
	int		len;

	is_escaped = false;
	litteral_depth = 0;
	current_lit = 0;
	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			is_escaped = true;
		else
		{
			if (is_escaped == true)
				len++;
			else if (str[i] == '\'' || str[i] == '\"')
			{
				if (str[i] != current_lit)
				{
					litteral_depth++;
					current_lit = str[i];
				}
				else
				{
					litteral_depth--;
					if (litteral_depth == 0)
						current_lit = 0;
					else if (current_lit == '\'')
						current_lit = '\"';
					else
						current_lit = '\'';
				}
			}
			else if (litteral_depth == 0 && is_escaped == false && str[i] == ' ')
				return len;
			else
				len++;
			is_escaped = false;
		}
		i++;
	}
}

char	*skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	return (str + i);
}

char	**split_args(char *args)
{
}