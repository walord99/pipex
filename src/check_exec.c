/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 02:34:20 by bplante/Wal       #+#    #+#             */
/*   Updated: 2023/12/18 14:44:01 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	relative(char **exec)
{
	if (access(exec[0], X_OK) != 0)
	{
		ft_printf_fd("pipex: %s: %s\n", 2, strerror(errno), exec[0]);
		if (access(exec[0], F_OK) != 0)
			return (126);
		return (127);
	}
	return (0);
}

int	env_path(char **exec, char *cmd)
{
	if (exec[0] && access(exec[0], X_OK) != 0)
	{
		ft_printf_fd("pipex: %s: %s\n", 2, strerror(errno), exec[0]);
		return (126);
	}
	else if (!exec[0])
	{
		ft_printf_fd("pipex: command not found: %s\n", 2, cmd);
		free(cmd);
		return (127);
	}
	return (0);
}

char	*get_path(char **exec, char **env)
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

int	check_exec(char **exec, char **env)
{
	char	*path;
	char	*temp;

	if (!exec)
	{
		ft_printf_fd("pipex: permission denied: \n", 2);
		return (126);
	}
	if (ft_strchr(exec[0], '/'))
		return (relative(exec));
	else
	{
		path = get_path(exec, env);
		temp = exec[0];
		exec[0] = path;
		return (env_path(exec, temp));
	}
}
