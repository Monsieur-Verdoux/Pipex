/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:12:19 by akovalev          #+#    #+#             */
/*   Updated: 2024/02/19 15:16:24 by akovalev         ###   ########.fr       */
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
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_all(t_pipex *p)
{
	if (p->paths != NULL)
		free_split(p->paths);
	if (p->cmd1 != NULL)
	{
		free(p->cmd1);
		p->cmd1 = NULL;
	}
	if (p->cmd2 != NULL)
	{
		free(p->cmd2);
		p->cmd1 = NULL;
	}
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
