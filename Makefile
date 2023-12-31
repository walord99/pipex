FILES			= 	main.c \
					pipes.c \
					check_exec.c \
					constructor.c \
					split_args/split_args.c \
					split_args/split_args_finders.c 

SRC_DIR			= 	src
OBJ_DIR			= 	obj
SRC				= 	$(addprefix $(SRC_DIR)/, $(FILES))
OBJ 			= 	$(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))
CC				= 	gcc

NAME			= 	pipex
HEADER_DIR		= 	include
LIBFT_DIR 		= 	libft
LIBFT			= 	$(LIBFT_DIR)/libft.a

INCLUDES		= 	-I$(HEADER_DIR) -I$(LIBFT_DIR)
#CC_DEBUG 		= 	-fsanitize=address -fno-omit-frame-pointer
#L_DEBUG		=	-lasan
ERROR_FLAGS 	= 	-Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJ)
	$(CC) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_DEBUG) $(INCLUDES) $(ERROR_FLAGS) -c $< -o $@ -g

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/split_args

$(LIBFT):
	make -C libft

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

fclean: clean
	make -C $(LIBFT_DIR) fclean

re: clean all

valgrind:
	valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all ./pipex in_test "" "grep er" out_test

test:
	gcc $(INCLUDES) test.c src/split_args.c -L$(LIBFT_DIR) -lft -g -o test

.PHONY: all clean fclean re test valgrind
