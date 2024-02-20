/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:30:31 by akovalev          #+#    #+#             */
/*   Updated: 2024/02/20 13:34:06 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H
# include "Libft/libft.h"
# include "Libft/get_next_line_bonus.h"
# include "Libft/ft_printf.h"
# include <fcntl.h>

typedef struct s_pipex
{
	int		input;
	int		output;
	int		argc;
	char	*cmd1;
	char	*cmd2;
	char	**argv;
	char	**env;
	char	**paths;
	char	**com_params_1;
	char	**com_params_2;
	int		pipefd[2];
	pid_t	pid;
	pid_t	pid2;
}	t_pipex;

void	free_split(char **arr);
void	free_all(t_pipex *p);
char	**parse_paths(char **env);
char	*check_command(t_pipex *p, int index);
void	initialize_struct(t_pipex *p, int argc, char **argv, char **env);

#endif