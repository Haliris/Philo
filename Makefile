# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/05 11:55:42 by jteissie          #+#    #+#              #
#    Updated: 2024/08/05 13:54:08 by jteissie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

CCFLAGS = -Wall -Wextra -Werror -pthread -g3

SRCDIR = src
OBJDIR = obj
INCLUDE = -I ./include

SRC_FILES = main.c \
			utils.c \

vpath %.c ./ src/

OBJ_FILES = $(addprefix $(OBJDIR)/, $(CFILES:.c=.o))

all: $(OBJDIR) $(NAME)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)
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
