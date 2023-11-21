/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:57:14 by bplante           #+#    #+#             */
/*   Updated: 2023/11/21 09:01:58 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <libft.h>
# include <stdarg.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define IN_FILE 0
# define OUT_OVER 1
# define OUT_APPEND 2

typedef struct s_pipe_pair
{
	int		read[2];
	int		write[2];
}			t_pipe_pair;

char		**split_args(char *args);
char		*get_path(char **env, char **exec);
int			dup_close_pipes(t_pipe_pair *pipe_pair, int extra_close[2]);
int			check_io_files(char *in, char *out, int fd[2]);
t_pipe_pair	*create_pipe_pair(int read0, int read1, int write0, int write1);
void		close_and_free(int fd_pipe[2], t_pipe_pair *pipe_paire);
int			check_exec(char **exec, char **env);
char		*find_arg_end(char *str);
char		*skip_spaces(char *str);
int			get_arg_len(char *str);
bool		is_char_real(char c, bool is_escaped, char current_lit);
bool		is_arg_ended(char c, bool is_escaped, char current_lit);
void		do_the_thing(char c, bool is_escaped, char *current_lit);

#endif