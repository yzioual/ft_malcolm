NAME := ft_malcolm

SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := libft

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

LIBFT := $(LIBFT_DIR)/libft.a
CC := gcc
CFLAGS := -Wall -Wextra -Werror -I$(SRC_DIR) -I$(LIBFT_DIR) -MMD -MP -g

LDFLAGS := -L$(LIBFT_DIR)
LDLIBS := -lft -lpthread
RM := rm -f

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	@echo "[ft_malcolm] Linking $(NAME)..."
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $(NAME) -lm
	@echo "[ft_malcolm] $(NAME) build complete!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@$(RM) -r $(OBJ_DIR)
	@echo "Cleaning..."
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Cleaning program executable..."
	@$(RM) $(NAME)
	@echo "Full cleaning..."
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

-include $(OBJS:.o=.d)
