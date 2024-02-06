/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:29:21 by akovalev          #+#    #+#             */
/*   Updated: 2024/02/06 13:29:57 by akovalev         ###   ########.fr       */
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
	free_split(p->paths);
	free_split(p->com_params);
	if (p->cmd1 != NULL)
		free(p->cmd1);
	if (p->cmd2 != NULL)
		free(p->cmd2);
	if (p->cmd1_params != NULL)
		free(p->cmd1_params);
	if (p->cmd2_params != NULL)
		free(p->cmd2_params);
}

char	**parse_paths(char **env)
{
	int		i;
	char	**paths;

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

	p->com_params = ft_split(p->argv[index], ' ');
	if (p->com_params[1] && p->com_params[2])
		return (NULL);
	i = 0;
	while (p->paths[i])
	{
		com_slash = ft_strjoin(p->paths[i], "/");
		command = ft_strjoin(com_slash, p->com_params[0]);
		free(com_slash);
		com_slash = NULL;
		if (access(command, X_OK) != -1)
		{
			ft_printf("Command is now: %s\n", command);
			if (index == 2 && p->com_params[1])
				p->cmd1_params = ft_strdup(p->com_params[1]);
			if (index == 3 && p->com_params[1])
				p->cmd2_params = ft_strdup(p->com_params[1]);
			ft_printf("Command params are now: %s, %s\n", p->cmd1_params, p->cmd2_params);
			free_split(p->com_params);
			return (command);
		}
		i++;
		free(command);
	}
	free_split(p->com_params);
	return (NULL);
}

int	validate_arguments(t_pipex *p)
{
	p->cmd1 = check_command(p, 2);
	if (!p->cmd1)
	{
		ft_printf("Invalid command #1\n");
		return (0);
	}
	p->cmd2 = check_command(p, 3);
	if (!p->cmd2)
	{
		ft_printf("Invalid command #2\n");
		return (0);
	}
	p->input = open(p->argv[1], O_RDONLY);
	if (p->input == -1)
	{
		perror("Failure opening file");
		return (0);
	}
	if (access(p->argv[4], F_OK) != -1)
	{
		ft_printf("Output file exists\n");
		if (access(p->argv[4], W_OK) != 0)
		{
			ft_printf("Output file exists but does not have write permissions\n");
			return (0);
		}
	}
	return (1);
}
void	initialize_struct(t_pipex *p, int argc, char **argv, char **env)
{
	p->argc = argc;
	p->argv = argv;
	p->env = env;
	p->cmd1 = NULL;
	p->cmd2 = NULL;
	p->cmd1_params = NULL;
	p->cmd2_params = NULL;
}

int	main(int argc, char **argv, char**env)
{
	t_pipex	p;

	initialize_struct(&p, argc, argv, env);
	if (argc != 5)
	{
		ft_printf("There should be 4 arguments\n");
		exit (EXIT_FAILURE);
	}
	p.paths = parse_paths(env);
	if (!validate_arguments(&p))
	{
		free_all(&p);
		return (0);
	}
	free_all(&p);
	return (1);
	//exit (EXIT_SUCCESS);
}
