# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: plertsir <plertsir@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/22 13:29:22 by kburalek          #+#    #+#              #
#    Updated: 2023/11/07 21:09:28 by plertsir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Library Name
NAME = libft.a

# Source Files
SRC = ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
	  ft_memcpy.c ft_memmove.c ft_memset.c ft_strlen.c ft_strlcpy.c ft_toupper.c \
	  ft_tolower.c ft_atoi.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_strnstr.c \
	  ft_strdup.c ft_calloc.c ft_memchr.c ft_memcmp.c ft_strlcat.c ft_substr.c \
	  ft_strjoin.c ft_strtrim.c ft_itoa.c ft_split.c ft_strmapi.c ft_striteri.c \
	  ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_malloczero.c

# Bonus Source Files (if applicable)
SRC_BONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
			ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstiter_bonus.c \
			ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstmap_bonus.c

# Object Directory
OBJ_DIR = objs/
OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

# Bonus Object Directory (if applicable)
OBJ_DIR_BONUS = objs_bonus/
OBJS_BONUS = $(addprefix $(OBJ_DIR_BONUS), $(SRC_BONUS:.c=.o))

# Default Target
all: $(NAME)

# Compilation Rule for C Files
$(OBJ_DIR)%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Target to Create the Library
$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

# Bonus Target (if applicable)
bonus: $(OBJS_BONUS)
	ar rcs $(NAME) $(OBJS_BONUS)

# Clean Up Object Files
clean:
	rm -rf $(OBJ_DIR)

# Clean Everything
fclean: clean
	rm -f $(NAME)

# Rebuild Everything
re: fclean all

# Rebuild with Bonus (if applicable)
rebonus: fclean bonus

# Phony Targets
.PHONY: clean fclean all re bonus rebonus
