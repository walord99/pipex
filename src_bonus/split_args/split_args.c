/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 02:15:04 by bplante           #+#    #+#             */
/*   Updated: 2023/11/22 13:04:59 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// once at none space char
// add 1 to len until none escaped space and depth == 0 space

#include "pipex_bonus.h"

// TODO, GO END WORD, BY SPLITTING THIS INTO USEFULL FUNCTION LIKE,
// BREAKOUT OF LITTERAL OR SMTH
bool	is_char_real(char c, bool is_escaped, char current_lit)
{
	if (is_escaped)
		return (true);
	if (current_lit == c || ((c == '\'' || c == '\"') && current_lit == 0))
		return (false);
	return (true);
}

bool	is_arg_ended(char c, bool is_escaped, char current_lit)
{
	if (!current_lit && c == ' ' && !is_escaped)
		return (true);
	return (false);
}

void	do_the_thing(char c, bool is_escaped, char *current_lit)
{
	if (*current_lit && c == *current_lit && !is_escaped)
		*current_lit = 0;
	else if (!*current_lit && (c == '\'' || c == '\"') && !is_escaped)
		*current_lit = c;
}

void	cpytrim_litterals(char *src, char *dest)
{
	int		i;
	bool	is_escaped;
	char	current_lit;
	int		j;

	is_escaped = false;
	current_lit = 0;
	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i] == '\\' && is_escaped == false)
			is_escaped = true;
		else if (is_arg_ended(src[i], is_escaped, current_lit) == 0)
		{
			if (is_char_real(src[i], is_escaped, current_lit))
				dest[j++] = src[i];
			do_the_thing(src[i], is_escaped, &current_lit);
			is_escaped = false;
		}
		else
			break ;
		i++;
	}
	dest[i] = 0;
}

char	**split_args(char *args)
{
	t_list	*split;
	char	**split_tab;

	if (!args)
		return (NULL);
	split = NULL;
	args = skip_spaces(args);
	while (*args)
	{
		split = ft_lstadd_back(split, ft_calloc(get_arg_len(args) + 1,
					sizeof(char)));
		cpytrim_litterals(args, ft_lstlast(split)->content);
		args = skip_spaces(find_arg_end(args));
	}
	split_tab = lst_to_tab(split);
	ft_lstclear(split, NULL);
	return (split_tab);
}
