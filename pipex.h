/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:30:31 by akovalev          #+#    #+#             */
/*   Updated: 2024/02/08 15:28:04 by akovalev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H
# include "Libft/libft.h"
# include "Libft/get_next_line_bonus.h"
# include "Libft/ft_printf.h"
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

typedef struct s_pipex
{
	int		input;
	int		output;
	int		rd_pipe;
	int		wr_pipe;
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
}	t_pipex;
#endif