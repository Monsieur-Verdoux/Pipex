/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalev <akovalev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:30:31 by akovalev          #+#    #+#             */
/*   Updated: 2024/02/06 13:02:47 by akovalev         ###   ########.fr       */
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
	char	*cmd1_params;
	char	*cmd2_params;
	char	**argv;
	char	**env;
	char	**paths;
	char	**com_params;
}	t_pipex;
#endif