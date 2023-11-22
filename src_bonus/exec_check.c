/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 08:45:48 by bplante           #+#    #+#             */
/*   Updated: 2023/11/22 15:38:06 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*get_path(char **env, char **exec);

int	check_exec(char **exec, char **env)
{
	char	*ptr;

	ptr = NULL;
	if (!exec)
		return (1);
	if (!ft_strchr(exec[0], '/'))
	{
		ptr = exec[0];
		exec[0] = get_path(env, exec);
	}
	if (exec[0] && access(exec[0], X_OK) != 0)
	{
		free(ptr);
		ft_printf_fd("pipex: %s: %s\n", 2, strerror(errno), exec[0]);
		return (-1);
	}
	else if (!exec[0])
	{
		ft_printf_fd("pipex: command not found: %s\n", 2, ptr);
		free(ptr);
		return (-1);
	}
	free(ptr);
	return (0);
}

char	*get_path(char **env, char **exec)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	paths = ft_split(get_env(env, "PATH"), ':');
	while (paths[i])
	{
		path = ft_strjoin_free(paths[i], ft_strjoin("/", exec[0]), FREE_S2);
		if (access(path, F_OK) == 0)
		{
			free_tab((void **)paths, &free);
			return (path);
		}
		free(path);
		i++;
	}
	free_tab((void **)paths, &free);
	return (NULL);
}
