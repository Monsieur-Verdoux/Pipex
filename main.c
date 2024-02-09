/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:29:21 by akovalev          #+#    #+#             */
/*   Updated: 2024/02/09 17:00:42 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_all(t_pipex *p)
{
	if (p->paths != NULL)
		free_split(p->paths);
	if (p->cmd1 != NULL)
		free(p->cmd1);
	if (p->cmd2 != NULL)
		free(p->cmd2);
	if (p->com_params_1 != NULL)
		free_split(p->com_params_1);
	if (p->com_params_2 != NULL)
		free_split(p->com_params_2);
}

char	**parse_paths(char **env)
{
	int		i;
	char	**paths;

	paths = NULL;
	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
			paths = ft_split(env[i] + 5, ':');
		i++;
	}
	return (paths);
}

char	*check_command(t_pipex *p, int index)
{
	char	*com_slash;
	char	*command;
	int		i;

	if (index == 2)
		p->com_params_1 = ft_split(p->argv[index], ' ');
	else if (index == 3)
		p->com_params_2 = ft_split(p->argv[index], ' ');
	i = 0;
	while (p->paths[i])
	{
		com_slash = ft_strjoin(p->paths[i], "/");
		if (index == 2)
			command = ft_strjoin(com_slash, p->com_params_1[0]);
		else if (index == 3)
			command = ft_strjoin(com_slash, p->com_params_2[0]);
		free(com_slash);
		com_slash = NULL;
		if (access(command, X_OK) != -1)
			return (command);
		i++;
		free(command);
	}
	return (NULL);
}

void	initialize_struct(t_pipex *p, int argc, char **argv, char **env)
{
	p->argc = argc;
	p->argv = argv;
	p->env = env;
	p->cmd1 = NULL;
	p->cmd2 = NULL;
	p->com_params_1 = NULL;
	p->com_params_2 = NULL;
	p->paths = NULL;
}

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
		perror("pipex");
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
		perror("pipex");
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
		ft_printf("Please provide exactly four arguments\n");
		exit (EXIT_FAILURE);
	}
	initialize_struct(p, argc, argv, env);
	p->paths = parse_paths(env);
	if (p->paths == NULL)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
	if (pipe(p->pipefd) == -1)
	{
		perror("pipe");
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
	forking (&p);
	close(p.pipefd[0]);
	close(p.pipefd[1]);
	if (waitpid(p.pid, &status, 0) == -1)
	{
		perror("waitpid");
		free_all(&p);
		exit(EXIT_FAILURE);
	}
	if (waitpid(p.pid2, &status, 0) == -1)
	{
		perror("waitpid");
		free_all(&p);
		exit(EXIT_FAILURE);
	}
	free_all(&p);
	return (WEXITSTATUS(status));
}
