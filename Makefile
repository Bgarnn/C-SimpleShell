NAME = minishell

FLAGS = -Wall -Wextra -Werror -fsanitize=address -g 
# FLAGS = -Wall -Wextra -Werror -g 


LIBFT = $(LIBFT_DIRECTORY)libft.a
LIBFT_DIRECTORY = ./libft/

# READLINE = /usr/local/opt/readline

READLINE = /opt/homebrew/Cellar/readline/8.2.1/

SRC = execute.c first_main.c fork_child.c \
	 get_path_cmd.c input_error.c\
	input_output.c mns.c utils.c built_in.c
OBJ_DIR = object/

OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@gcc $(FLAGS) -I$(READLINE)/include -L$(READLINE)/lib $(OBJS) $(LIBFT) -o $(NAME) -lreadline -lncurses
	@echo " ------------  MINISHELL ------------ "
$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c
	@gcc $(FLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIRECTORY)

clean:
	@$(MAKE) -C $(LIBFT_DIRECTORY) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(LIBFT)
	@rm -rf $(NAME)

re: fclean all
