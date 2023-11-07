/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bplante <bplante@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:49:27 by bplante           #+#    #+#             */
/*   Updated: 2023/11/07 15:16:29 by bplante          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv)
{
	pid_t pid;
	pid = fork();
    if (pid < 0)
        perror("Failed to fork.");
    else if (pid == 0)
    {
		char *t[] = {NULL};
		char *test[] = {"/usr/bin/wc", "src/main.c", NULL};
		execve(test[0], test, t);
	}
	else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            printf("Parent: Process ID %ld Exit status of the child was %d\n", (long)getpid, exit_status);
        }
    }
    return 0;
}
