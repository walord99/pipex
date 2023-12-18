/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:57:14 by bplante           #+#    #+#             */
/*   Updated: 2023/12/18 14:57:08 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <libft.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define READ 0
# define WRITE 1
# define NOT_PIPE -2

typedef struct s_pipes_tab
{
	int	len;
	int	*pipes;
}		t_pipe_tab;

char	**split_args(char *args);
char	*skip_spaces(char *str);
char	*find_arg_end(char *str);
int		get_arg_len(char *str);

void	close_all_pipes(t_pipe_tab *pipe_pair);
int		create_pipes_tab(t_pipe_tab *pipe_tab, int argc, char **argv);
int		generate_pipes(t_pipe_tab *pipes_tab);
void	close_unused_pipes(t_pipe_tab *pipe_tab, int pos);

bool	is_arg_ended(char c, bool is_escaped, char current_lit);
void	do_the_thing(char c, bool is_escaped, char *current_lit);
bool	is_char_real(char c, bool is_escaped, char current_lit);

int		check_exec(char **exec, char **env);

void	allocate_pipe_tab(t_pipe_tab *pipe_tab, int size);

#endif