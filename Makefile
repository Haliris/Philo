# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/05 11:55:42 by jteissie          #+#    #+#              #
#    Updated: 2024/08/05 17:54:10 by jteissie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

MAX_PHILO = 200
CCFLAGS = -Wall -Wextra -Werror -pthread -g3 -DMAX_PHILO=$(MAX_PHILO)

SRCDIR = src
OBJDIR = obj
INCLUDE = -I ./include

SRC_FILES = main.c \
			utils.c \
			mutex_handler.c \

vpath %.c ./ src/

OBJ_FILES = $(addprefix $(OBJDIR)/, $(SRC_FILES:.c=.o))

all: $(OBJDIR) $(NAME)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJ_FILES)
	$(CC) $(CCFLAGS) $(INCLUDE) -o $(NAME) $(OBJ_FILES)
	@echo "> Philosophers ready to think!"

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
