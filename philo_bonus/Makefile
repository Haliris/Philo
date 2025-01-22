# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jteissie <jteissie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/05 11:55:42 by jteissie          #+#    #+#              #
#    Updated: 2024/08/13 14:39:09 by jteissie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = cc

MAX_PHILO = 200

CCFLAGS = -Wall -Wextra -Werror -g3 -DMAX_PHILO=$(MAX_PHILO) -pthread

SRCDIR = src
OBJDIR = obj
INCLUDE = -I ./include_bonus
SRC_FILES = main_bonus.c \
			utils_bonus.c \
			sem_handler.c \
			create_philos_bonus.c \
			monitor_philos_bonus.c \
			time_utils_bonus.c \
			philo_routine_bonus.c \
			philo_routine_handler.c \
			philo_routine_threads.c \

vpath %.c ./ src_bonus/

OBJ_FILES = $(addprefix $(OBJDIR)/, $(SRC_FILES:.c=.o))

all: $(OBJDIR) $(NAME)

debug: CCFLAGS += -fsanitize=thread
debug: all

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

.PHONY: all clean fclean re debug
