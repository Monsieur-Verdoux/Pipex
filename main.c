/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:29:21 by akovalev          #+#    #+#             */
/*   Updated: 2024/02/19 15:15:37 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_child(t_pipex *p)
{
	close(p->pipefd[0]);
	p->input = open(p->argv[1], O_RDONLY);
	if (p->input == -1)
	{
		perror("Failure opening file");
		free_all(p);
		exit (EXIT_FAILURE);
	}
	p->cmd1 = check_command(p, 2);
	if (!p->cmd1)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putstr_fd(p->argv[2], 2);
		ft_putchar_fd('\n', 2);
		free_all(p);
		exit (EXIT_FAILURE);
	}
	dup2(p->pipefd[1], STDOUT_FILENO);
	dup2(p->input, STDIN_FILENO);
	close(p->pipefd[1]);
	close(p->input);
	execve(p->cmd1, p->com_params_1, p->env);
	perror("execve");
	free_all(p);
	exit (EXIT_FAILURE);
}

void	second_child(t_pipex *p)
{
	close(p->pipefd[1]);
	p->output = open(p->argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (p->output == -1)
	{
		perror("Failure opening file");
		free_all(p);
		exit (EXIT_FAILURE);
	}
	p->cmd2 = check_command(p, 3);
	if (!p->cmd2)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putstr_fd(p->argv[3], 2);
		ft_putchar_fd('\n', 2);
		free_all(p);
		exit (EXIT_FAILURE);
	}
	dup2(p->pipefd[0], STDIN_FILENO);
	dup2(p->output, STDOUT_FILENO);
	close(p->pipefd[0]);
	close(p->output);
	execve(p->cmd2, p->com_params_2, p->env);
	perror("execve");
	free_all(p);
	exit (EXIT_FAILURE);
}

void	initial_checks(t_pipex *p, int argc, char **argv, char **env)
{
	if (argc != 5)
	{
		ft_putstr_fd("Please provide exactly four arguments\n", 2);
		exit (EXIT_FAILURE);
	}
	if (!argv[1][0] || !argv[4][0])
	{
		ft_putstr_fd("no such file or directory:\n", 2);
		exit(EXIT_FAILURE);
	}
	if (!argv[2][0] || !argv[3][0])
	{
		ft_putstr_fd("command not found:\n", 2);
		exit(EXIT_FAILURE);
	}
	initialize_struct(p, argc, argv, env);
	p->paths = parse_paths(env);
	if (p->paths == NULL)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
}

void	forking(t_pipex *p)
{
	p->pid = fork();
	if (p->pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (p->pid == 0)
		first_child(p);
	p->pid2 = fork();
	if (p->pid2 < 0)
	{
		perror("fork");
		free_all(p);
		exit(EXIT_FAILURE);
	}
	if (p->pid2 == 0)
		second_child(p);
}

int	main(int argc, char **argv, char**env)
{
	t_pipex	p;
	int		status;

	initial_checks(&p, argc, argv, env);
	if (pipe(p.pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	forking (&p);
	close(p.pipefd[0]);
	close(p.pipefd[1]);
	if (waitpid(p.pid, &status, 0) == -1)
	{
		free_all(&p);
		exit(EXIT_FAILURE);
	}
	if (waitpid(p.pid2, &status, 0) == -1)
	{
		free_all(&p);
		exit(EXIT_FAILURE);
	}
	free_all(&p);
	return (WEXITSTATUS(status));
}
