/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:57:14 by bplante           #+#    #+#             */
/*   Updated: 2023/11/21 08:05:38 by bplante          ###   ########.fr       */
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

char	**split_args(char *args);

#endif