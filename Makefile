NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I./include -o3

GREEN = \033[0;32m
BLUE = \033[0;34m
PURPLE = \033[0;35m
COLOUR_END = \033[0m

OBJ_DIR = ./obj
SRC_DIR = ./src
LIBFT_DIR = ./libft
LIBFT	:= libft/libft.a
MLX_LIBS += -L./minilibx-linux -lmlx -lXext -lX11 -lm -lbsd
MAKEFLAGS += -s

GNL = ./gnl/get_next_line_utils.c ./gnl/get_next_line.c
MAIN = ./src/main.c ./src/read_map.c ./src/ft_init.c ./src/utils.c ./src/ft_render.c

SRC = $(GNL) $(MAIN)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(MAKE) -C ./minilibx-linux/
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_LIBS) -o $(NAME)
	@echo "$(PURPLE)EVERYTHING COMPILED!$(COLOUR_END)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@mkdir -p ./obj/libft
	@mkdir -p ./obj/gnl
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@make clean -C minilibx-linux/
	@make clean -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)EVERYTHING CLEANED!$(COLOUR_END)"

fclean: clean
	@rm -f $(NAME)
	@$(RM) -f $(minilibx-linux/)libmlx.a
	@make fclean -C $(LIBFT_DIR)
	@echo "$(BLUE)EVERYTHING CLEANED COMPLETELY!$(COLOUR_END)"

re: fclean all

.PHONY: all clean fclean re
