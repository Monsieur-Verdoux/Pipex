# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akovalev <akovalev@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/05 17:39:09 by akovalev          #+#    #+#              #
#    Updated: 2024/02/19 15:18:00 by akovalev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= pipex 

CFLAGS	:= -Wextra -Wall -Werror -Wunreachable-code -Ofast 

SRCS	:= main.c \
			tools.c

OBJS	:= ${SRCS:.c=.o} Libft/libft.a

BONUS_OBJS := ${BONUS_SRCS:.c=.o} Libft/libft.a 

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< 

$(NAME): $(OBJS) 
	@$(CC) $(OBJS) -o $(NAME)

Libft/libft.a: 
	@$(MAKE) -C Libft/ all

clean:
	@rm -rf $(OBJS)
	@$(MAKE) -C Libft/ clean

fclean: clean
	@rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re 