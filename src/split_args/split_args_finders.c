/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args_finders.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 08:55:15 by bplante           #+#    #+#             */
/*   Updated: 2023/11/21 09:03:15 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	get_arg_len(char *str)
{
	int		i;
	bool	is_escaped;
	char	current_lit;
	int		len;

	is_escaped = false;
	current_lit = 0;
	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && is_escaped == false)
			is_escaped = true;
		else if (is_arg_ended(str[i], is_escaped, current_lit) == 0)
		{
			do_the_thing(str[i], is_escaped, &current_lit);
			if (is_char_real(str[i], is_escaped, current_lit))
				len++;
			is_escaped = false;
		}
		else
			return (len);
		i++;
	}
	return (len);
}

char	*find_arg_end(char *str)
{
	int		i;
	bool	is_escaped;
	char	current_lit;

	is_escaped = false;
	current_lit = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && is_escaped == false)
			is_escaped = true;
		else if (is_arg_ended(str[i], is_escaped, current_lit) == 0)
		{
			do_the_thing(str[i], is_escaped, &current_lit);
			is_escaped = false;
		}
		else
			return (&str[i]);
		i++;
	}
	return (&str[i]);
}

char	*skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	return (str + i);
}
