NAME = minishell

FLAGS = -Wall -Wextra -Werror
# FLAGS = -fsanitize=address -g

LIBFT = $(LIBFT_DIRECTORY)libft.a
LIBFT_DIRECTORY = ./libft/

# READLINE = /usr/local/opt/readline

READLINE = /opt/homebrew/Cellar/readline/8.2.1/

SRC = execute.c first_main.c fork_child.c get_path_cmd.c input_error.c\
	input_output.c utils.c cd_built_in.c exit_built_in.c\
	export_built_in.c check_built_in.c built_in_utils.c pwd_built_in.c\
	export_utils.c env_built_in.c echo_built_in.c built_in_error.c\
	unset_built_in.c handle_memory.c free_grouped_token.c execute_utils.c\
	cd_update_env.c cd_utils.c execute_bash.c get_path_utils.c utils2.c\
	export_unset_utils2.c minishell.c mns1-mns_init.c mns2-init_utils.c mns3-main_while.c\
	mns4-input_to_token.c mns5-input_to_token.c mns6-input_to_token.c\
	mns7-input_to_token.c mns8-token_to_organize.c mns9-mns_free.c

OBJ_DIR = object/

OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	gcc $(FLAGS) -I$(READLINE)/include -L$(READLINE)/lib $(OBJS) $(LIBFT) -o $(NAME) -lreadline -lncurses

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c
	gcc $(FLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIRECTORY)

clean:
	$(MAKE) -C $(LIBFT_DIRECTORY) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(LIBFT)
	rm -rf $(NAME)

re: fclean all
