/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 11:09:36 by rbenoit           #+#    #+#             */
/*   Updated: 2024/03/17 11:09:38 by rbenoit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

static void	check_file(char **av)
{
	if (is_directory(av[0]))
	{
		write(2, "minishell: ", 11);
		write(2, av[0], ft_strlen(av[0]));
		write(2, ": Is a directory\n", 17);
		exit(126);
	}
	if (access(av[0], F_OK | X_OK) == -1)
	{
		if (errno != EACCES)
		{
			write(2, "minishell: ", 11);
			perror(av[0]);
			exit(127);
		}
	}
}

int	run(char **av, t_shell *shell)
{
	int	id;
	int	status;

	id = fork();
	if (id == 0)
	{
		close(shell->stdin_b);
		close(shell->stdout_b);
		check_file(av);
		if (execve(av[0], av, shell->envp) == -1)
		{
			write(2, "minishell: ", 11);
			perror(av[0]);
			exit(126);
		}
	}
	else
	{
		waitpid(id, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
	}
	return (status);
}
